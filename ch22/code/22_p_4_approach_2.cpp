#include <list>
#include <vector>

enum Color { kWhite, kNonWhite };

struct OriginalVertex
{
    Color color;
    int value;// L
    int min;
};

struct TranposeVertex
{
    Color color;
};

template <typename VertexType>
struct Graph
{
    int size;
    std::vector< std::list<int> > adj;
    std::vector< VertexType > vertices;
    
    Graph(int size) : size(size), adj(size), vertices(size) {}

    void WhitenAllVertices()
    {
        for (VertexType u : vertices)
        {
            u.color = Color::kWhite;
        }
    }
};

Graph<TranposeVertex> Tranpose(const Graph<OriginalVertex>& graph)
{
    int u;
    Graph<TranposeVertex> result(graph.size);
    for (u = 0; u < graph.size; ++u)
    {
        for (int v : graph.adj[u])
        {
            result.adj[v].push_back(u);
        }
    }
    return result;
}

void DFSVisit(Graph<TranposeVertex>& tranpose, int u, 
    Graph<OriginalVertex>& graph, int root)
{
    tranpose.vertices[u].color = Color::kNonWhite;
    graph.vertices[u].min = root;
    for (int v : tranpose.adj[u])
    {
        if (tranpose.vertices[v].color == Color::kWhite)
            DFSVisit(tranpose, v, graph, root);
    }
}

void Reachability(Graph<OriginalVertex>& graph)
{
    int u;
    std::vector<int> counting_sort(graph.size);
    // sort vertices in the increasing order of L value in linear time
    for (u = 0; u < graph.size; ++u)
    {
        counting_sort[graph.vertices[u].value] = u;
    }
    // find tranpose
    Graph<TranposeVertex> tranpose = Tranpose(graph);
    // DFS
    tranpose.WhitenAllVertices();
    for (int v : counting_sort)
    {
        if (tranpose.vertices[v].color == Color::kWhite)
            DFSVisit(tranpose, v, graph, v);
    }
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("example")
{
    std::cout << "-------------------\n";
    Graph<OriginalVertex> graph(8);
    graph.adj = {
        { 1 },// 0
        { 2, 4, 5 },// 1
        { 3, 6 },// 2
        { 2, 7 },// 3
        { 0, 5 },// 4
        { 6 },// 5
        { 5, 7 },// 6
        { 7 }// 7
    };
    graph.vertices[0].value = 3;
    graph.vertices[1].value = 6;
    graph.vertices[2].value = 7;
    graph.vertices[3].value = 1;
    graph.vertices[4].value = 0;
    graph.vertices[5].value = 4;
    graph.vertices[6].value = 2;
    graph.vertices[7].value = 5;
    clock_t start = clock();
    Reachability(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    REQUIRE(graph.vertices[0].min == 4);
    REQUIRE(graph.vertices[1].min == 4);
    REQUIRE(graph.vertices[2].min == 3);
    REQUIRE(graph.vertices[3].min == 3);
    REQUIRE(graph.vertices[4].min == 4);
    REQUIRE(graph.vertices[5].min == 6);
    REQUIRE(graph.vertices[6].min == 6);
    REQUIRE(graph.vertices[7].min == 7);
}
