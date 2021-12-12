#include <vector>
#include <list>

using AdjList = std::list<int>;

enum Color { kWhite, kGray, kBlack };

struct Vertex
{
    Color color;
    int prev;
    int discovery_time;
    int finishing_time;
};

struct Graph
{
    int size;
    std::vector< AdjList > adj;
    std::vector< Vertex > vertices;
    
    Graph() {}

    Graph(int size) : size(size), adj(size), vertices(size) {}
};

void DFSVisit(Graph& graph, int u_idx, int& time, 
    std::list<int>& topo_sort)
{
    Vertex& u = graph.vertices[u_idx];
    ++time;
    u.discovery_time = time;
    u.color = Color::kGray;
    for (int v_idx : graph.adj[u_idx])
    {
        Vertex& v = graph.vertices[v_idx];
        if (v.color == Color::kWhite)
        {
            v.prev = u_idx;
            DFSVisit(graph, v_idx, time, topo_sort);
        }
    }
    u.color = Color::kBlack;
    ++time;
    u.finishing_time = time;
    topo_sort.push_front(u_idx);
}

std::list< std::list<int> > DFSWithOrder(Graph& graph, 
    const std::list<int>& order)
{
    int time;
    std::list< std::list<int> > components;
    for (Vertex& u : graph.vertices)
    {
        u.color = Color::kWhite;
        u.prev = -1;
    }
    time = 0;
    for (int u_idx : order)
    {
        if (graph.vertices[u_idx].color == Color::kWhite)
        {
            components.emplace_back(std::list<int>());
            DFSVisit(graph, u_idx, time, components.back());
        }
    }
    return components;
}

std::list<int> TopologicalSort(Graph& graph)
{
    int time, u_idx;
    std::list<int> topo_sort;
    for (Vertex& u : graph.vertices)
    {
        u.color = Color::kWhite;
        u.prev = -1;
    }
    time = 0;
    for (u_idx = 0; u_idx < graph.size; ++u_idx)
    {
        if (graph.vertices[u_idx].color == Color::kWhite)
        {
            DFSVisit(graph, u_idx, time, topo_sort);
        }
    }
    return topo_sort;
}

Graph Transpose(const Graph& graph)
{
    size_t u;
    Graph target(graph.size);
    for (u = 0; u < graph.size; ++u)
    {
        for (int v : graph.adj[u])
        {
            target.adj[v].push_back(u);
        }
    }
    return target;
}

std::list< std::list<int> > StronglyConnectedComponents(Graph& graph)
{
    std::list<int> topo_sort = TopologicalSort(graph);
    Graph tranpose = Transpose(graph);
    std::list< std::list<int> > components = 
        DFSWithOrder(tranpose, topo_sort);
    return components;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("example")
{
    std::cout << "-------------------\n";
    Graph graph(8);
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
    clock_t start = clock();
    std::list< std::list<int> > components = 
        StronglyConnectedComponents(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    std::list< std::list<int> > solution = 
        { { 0, 4, 1 }, { 2, 3 }, { 6, 5 }, { 7 } };
    REQUIRE(components == solution);
}
