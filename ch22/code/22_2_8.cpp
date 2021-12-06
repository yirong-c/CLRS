#include <limits.h>
#include <vector>
#include <list>
#include <queue>

using AdjList = std::list<int>;

struct Vertex
{
    int depth;
    int prev;
    int height;
};

struct Graph
{
    std::vector< AdjList > adj;
    std::vector< Vertex > vertices;

    Graph() {}

    Graph(int size) : adj(size), vertices(size) {}
};

int DiameterAux(Graph& graph, int subtree_root_idx)
{
    int first_largest_h, second_largest_h, longest_path_length;
    first_largest_h = -1;
    second_largest_h = -1;
    longest_path_length = INT_MIN;
    Vertex& subtree_root = graph.vertices[subtree_root_idx];
    for (int child_idx : graph.adj[subtree_root_idx])
    {
        if (child_idx != subtree_root.prev)
        {
            Vertex& child = graph.vertices[child_idx];
            longest_path_length = std::max(longest_path_length, 
                DiameterAux(graph, child_idx));
            if (child.height > first_largest_h)
            {
                second_largest_h = first_largest_h;
                first_largest_h = child.height;
            }
            else if (child.height > second_largest_h)
            {
                second_largest_h = child.height;
            }
        }
    }
    subtree_root.height = first_largest_h + 1;
    longest_path_length = std::max(longest_path_length, 
                first_largest_h + second_largest_h + 2);
    return longest_path_length;
}

int Diameter(Graph& graph)
{
    int u_idx, v_idx;
    std::queue<int> q;
    // let vertex 0 be the root node
    graph.vertices[0].depth = 0;
    graph.vertices[0].prev = -1;
    q.push(0);
    while (q.empty() == false)
    {
        u_idx = q.front();
        q.pop();
        Vertex& u = graph.vertices[u_idx];
        for (int v_idx : graph.adj[u_idx])
        {
            if (v_idx != u.prev)
            {
                Vertex& v = graph.vertices[v_idx];
                v.depth = u.depth + 1;
                v.prev = u_idx;
                q.push(v_idx);
            }
        }
    }
    return DiameterAux(graph, 0);
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("example")
{
    std::cout << "-------------------\n";
    Graph graph(12);
    graph.adj = {
        { 1, 3 },// 0
        { 0 },// 1
        { 4 },// 2
        { 0, 7, 8 },// 3
        { 2, 7, 11 },// 4
        { 8, 9 },// 5
        { 8 },// 6
        { 3, 4, 10 },// 7
        { 3, 5, 6 },// 8
        { 5 },// 9
        { 7 },// 10
        { 4 }// 11
    };
    clock_t start = clock();
    int result = Diameter(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    REQUIRE(result == 6);
}
