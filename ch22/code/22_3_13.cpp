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

bool DFSVisit(Graph& graph, int u_idx, int& time)
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
            DFSVisit(graph, v_idx, time);
        }
        else if (v.color == Color::kBlack)
        {
            return false;
        }
    }
    u.color = Color::kBlack;
    ++time;
    u.finishing_time = time;
    return true;
}

bool DFSWithSrc(Graph& graph, int src_vertex)
{
    int time, u_idx;
    for (Vertex& u : graph.vertices)
    {
        u.color = Color::kWhite;
        u.prev = -1;
    }
    time = 0;
    return DFSVisit(graph, src_vertex, time);
}

bool CheckSinglyConnected(Graph& graph)
{
    int u_idx;
    for (u_idx = 0; u_idx < graph.size; ++u_idx)
    {
        if (DFSWithSrc(graph, u_idx) == false)
            return false; 
    }
    return true;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("1")
{
    std::cout << "-------------------\n";
    Graph graph(6);
    graph.adj = {
        { 1, 3 },// 0
        { 4 },// 1
        { 4, 5 },// 2
        { 1 },// 3
        { 3 },// 4
        { 5 }// 5
    };
    clock_t start = clock();
    bool result = CheckSinglyConnected(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    REQUIRE(result == false);
}

TEST_CASE("2")
{
    std::cout << "-------------------\n";
    Graph graph(6);
    graph.adj = {
        { 1, 2, 3 },// 0
        { 0 },// 1
        {},// 2
        { 4 },// 3
        { 0 }// 4
    };
    clock_t start = clock();
    bool result = CheckSinglyConnected(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    REQUIRE(result == true);
}
