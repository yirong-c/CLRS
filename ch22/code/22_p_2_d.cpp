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
    int low;
};

struct Graph
{
    int size;
    std::vector< AdjList > adj;
    std::vector< Vertex > vertices;
    
    Graph() {}

    Graph(int size) : size(size), adj(size), vertices(size) {}
};

void DFSVisit(Graph& graph, int u_idx, int& time, std::list<int>& articulations)
{
    bool is_articulation;
    int tree_edges_counter;
    Vertex& u = graph.vertices[u_idx];
    ++time;
    u.discovery_time = time;
    u.color = Color::kGray;
    u.low = u.discovery_time;
    is_articulation = false;
    tree_edges_counter = 0;
    for (int v_idx : graph.adj[u_idx])
    {
        Vertex& v = graph.vertices[v_idx];
        if (v.color == Color::kWhite)
        {
            // tree edge
            ++tree_edges_counter;
            v.prev = u_idx;
            DFSVisit(graph, v_idx, time, articulations);
            u.low = std::min(u.low, v.low);
            if (v.low >= u.discovery_time)
                is_articulation = true;
        }
        else if (v.color == Color::kGray && u.prev != v_idx)
        {
            // back edge
            u.low = std::min(u.low, v.low);
        }
    }
    u.color = Color::kBlack;
    ++time;
    u.finishing_time = time;
    if ((u.discovery_time != 1 && is_articulation) || 
        (u.discovery_time == 1 && tree_edges_counter >= 2))
        articulations.push_back(u_idx);
}

std::list<int> DFS(Graph& graph)
{
    int time, u_idx;
    std::list<int> articulations;
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
            DFSVisit(graph, u_idx, time, articulations);
        }
    }
    return articulations;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("1")
{
    std::cout << "-------------------\n";
    Graph graph(7);
    graph.adj = {
        { 1, 6 },// 0
        { 0, 2, 4 },// 1
        { 1, 3, 6 },// 2
        { 2, 4, 5 },// 3
        { 1, 3 },// 4
        { 3, 6 },// 5
        { 0, 2, 5 }// 6
    };
    clock_t start = clock();
    std::list<int> result = DFS(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    REQUIRE(graph.vertices[0].discovery_time == 1);
    REQUIRE(graph.vertices[0].finishing_time == 14);
    REQUIRE(graph.vertices[0].prev == -1);
    REQUIRE(graph.vertices[0].low == 1);
    REQUIRE(graph.vertices[1].discovery_time == 2);
    REQUIRE(graph.vertices[1].finishing_time == 13);
    REQUIRE(graph.vertices[1].prev == 0);
    REQUIRE(graph.vertices[1].low == 1);
    REQUIRE(graph.vertices[2].discovery_time == 3);
    REQUIRE(graph.vertices[2].finishing_time == 12);
    REQUIRE(graph.vertices[2].prev == 1);
    REQUIRE(graph.vertices[2].low == 1);
    REQUIRE(graph.vertices[3].discovery_time == 4);
    REQUIRE(graph.vertices[3].finishing_time == 11);
    REQUIRE(graph.vertices[3].prev == 2);
    REQUIRE(graph.vertices[3].low == 1);
    REQUIRE(graph.vertices[4].discovery_time == 5);
    REQUIRE(graph.vertices[4].finishing_time == 6);
    REQUIRE(graph.vertices[4].prev == 3);
    REQUIRE(graph.vertices[4].low == 2);
    REQUIRE(graph.vertices[5].discovery_time == 7);
    REQUIRE(graph.vertices[5].finishing_time == 10);
    REQUIRE(graph.vertices[5].prev == 3);
    REQUIRE(graph.vertices[5].low == 1);
    REQUIRE(graph.vertices[6].discovery_time == 8);
    REQUIRE(graph.vertices[6].finishing_time == 9);
    REQUIRE(graph.vertices[6].prev == 5);
    REQUIRE(graph.vertices[6].low == 1);
    std::list<int> solution = { };
    REQUIRE(result == solution);
}

TEST_CASE("2")
{
    std::cout << "-------------------\n";
    Graph graph(7);
    graph.adj = {
        { 1 },// 0
        { 0, 2, 4 },// 1
        { 1, 3, 6 },// 2
        { 2, 4, 5 },// 3
        { 1, 3 },// 4
        { 3, 6 },// 5
        { 2, 5 }// 6
    };
    clock_t start = clock();
    std::list<int> result = DFS(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    REQUIRE(graph.vertices[0].discovery_time == 1);
    REQUIRE(graph.vertices[0].finishing_time == 14);
    REQUIRE(graph.vertices[0].prev == -1);
    REQUIRE(graph.vertices[0].low == 1);
    REQUIRE(graph.vertices[1].discovery_time == 2);
    REQUIRE(graph.vertices[1].finishing_time == 13);
    REQUIRE(graph.vertices[1].prev == 0);
    REQUIRE(graph.vertices[1].low == 2);
    REQUIRE(graph.vertices[2].discovery_time == 3);
    REQUIRE(graph.vertices[2].finishing_time == 12);
    REQUIRE(graph.vertices[2].prev == 1);
    REQUIRE(graph.vertices[2].low == 2);
    REQUIRE(graph.vertices[3].discovery_time == 4);
    REQUIRE(graph.vertices[3].finishing_time == 11);
    REQUIRE(graph.vertices[3].prev == 2);
    REQUIRE(graph.vertices[3].low == 2);
    REQUIRE(graph.vertices[4].discovery_time == 5);
    REQUIRE(graph.vertices[4].finishing_time == 6);
    REQUIRE(graph.vertices[4].prev == 3);
    REQUIRE(graph.vertices[4].low == 2);
    REQUIRE(graph.vertices[5].discovery_time == 7);
    REQUIRE(graph.vertices[5].finishing_time == 10);
    REQUIRE(graph.vertices[5].prev == 3);
    REQUIRE(graph.vertices[5].low == 3);
    REQUIRE(graph.vertices[6].discovery_time == 8);
    REQUIRE(graph.vertices[6].finishing_time == 9);
    REQUIRE(graph.vertices[6].prev == 5);
    REQUIRE(graph.vertices[6].low == 3);
    std::list<int> solution = { 1 };
    REQUIRE(result == solution);
}

TEST_CASE("example")
{
    std::cout << "-------------------\n";
    Graph graph(23);
    graph.adj = {
        { 1, 2, 3 },// 0
        { 0, 2, 3 },// 1
        { 0, 1, 3, 21 },// 2
        { 0, 1, 2 },// 3
        { 5, 21 },// 4
        { 4, 21 },// 5
        { 7, 21 },// 6
        { 6, 8, 21 },// 7
        { 7 },// 8
        { 10, 12, 21 },// 9
        { 9, 11 },// 10
        { 10, 12, 22 },// 11
        { 9, 11 },// 12
        { 14, 16, 17, 18, 22 },// 13
        { 13, 15, 16, 22 },// 14
        { 14, 16 },// 15
        { 13, 14, 15 },// 16
        { 13 },// 17
        { 13, 19, 20 },// 18
        { 18, 20 },// 19
        { 18, 19 },// 20
        { 2, 4, 5, 6, 7, 9 },// 21
        { 11, 13, 14 },// 22
    };
    clock_t start = clock();
    std::list<int> result = DFS(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    std::list<int> solution = { 7, 18, 13, 22, 11, 9, 21, 2 };
    REQUIRE(result == solution);
}
