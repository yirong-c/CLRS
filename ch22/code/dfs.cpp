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

void DFSVisit(Graph& graph, int u_idx, int& time)
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
    }
    u.color = Color::kBlack;
    ++time;
    u.finishing_time = time;
}

void DFS(Graph& graph)
{
    int time, u_idx;
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
            DFSVisit(graph, u_idx, time);
        }
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
    DFS(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    REQUIRE(graph.vertices[0].discovery_time == 1);
    REQUIRE(graph.vertices[0].finishing_time == 8);
    REQUIRE(graph.vertices[0].prev == -1);
    REQUIRE(graph.vertices[1].discovery_time == 2);
    REQUIRE(graph.vertices[1].finishing_time == 7);
    REQUIRE(graph.vertices[1].prev == 0);
    REQUIRE(graph.vertices[2].discovery_time == 9);
    REQUIRE(graph.vertices[2].finishing_time == 12);
    REQUIRE(graph.vertices[2].prev == -1);
    REQUIRE(graph.vertices[3].discovery_time == 4);
    REQUIRE(graph.vertices[3].finishing_time == 5);
    REQUIRE(graph.vertices[3].prev == 4);
    REQUIRE(graph.vertices[4].discovery_time == 3);
    REQUIRE(graph.vertices[4].finishing_time == 6);
    REQUIRE(graph.vertices[4].prev == 1);
    REQUIRE(graph.vertices[5].discovery_time == 10);
    REQUIRE(graph.vertices[5].finishing_time == 11);
    REQUIRE(graph.vertices[5].prev == 2);
}
