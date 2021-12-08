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

struct Edge
{
    int vertex_from;
    int vertex_to;
    enum Type { kTree, kBack, kForward, kCross } type;

    Edge(int vertex_from, int vertex_to, Type type) 
        : vertex_from(vertex_from), vertex_to(vertex_to), type(type) {}
};

void DFSVisit(Graph& graph, int u_idx, int& time, std::list<Edge>& result)
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
            result.emplace_back(u_idx, v_idx, Edge::kTree);
            v.prev = u_idx;
            DFSVisit(graph, v_idx, time, result);
        }
        else if (v.color == Color::kGray)
        {
            result.emplace_back(u_idx, v_idx, Edge::kBack);
        }
        else if (u.discovery_time < v.discovery_time)
        {
            result.emplace_back(u_idx, v_idx, Edge::kForward);
        }
        else
        {
            result.emplace_back(u_idx, v_idx, Edge::kCross);
        }
    }
    u.color = Color::kBlack;
    ++time;
    u.finishing_time = time;
}

std::list<Edge> DFS(Graph& graph)
{
    int time, u_idx;
    std::list<Edge> result;
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
            DFSVisit(graph, u_idx, time, result);
        }
    }
    return result;
}

void DFSVisitUndirected(Graph& graph, int u_idx, int& time, std::list<Edge>& result)
{
    Vertex& u = graph.vertices[u_idx];
    ++time;
    u.discovery_time = time;
    u.color = Color::kGray;
    for (int v_idx : graph.adj[u_idx])
    {
        Vertex& v = graph.vertices[v_idx];
        if (u.prev != v_idx)
        {
            if (v.color == Color::kWhite)
            {
                result.emplace_back(u_idx, v_idx, Edge::kTree);
                v.prev = u_idx;
                DFSVisitUndirected(graph, v_idx, time, result);
            }
            else if (v.color == Color::kGray)
            {
                result.emplace_back(u_idx, v_idx, Edge::kBack);
            }
        }
    }
    u.color = Color::kBlack;
    ++time;
    u.finishing_time = time;
}

std::list<Edge> DFSUndirected(Graph& graph)
{
    int time, u_idx;
    std::list<Edge> result;
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
            DFSVisitUndirected(graph, u_idx, time, result);
        }
    }
    return result;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("directed")
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
    std::list<Edge> result = DFS(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    for (Edge e : result)
    {
        std::cout << "(" << e.vertex_from << ", " << e.vertex_to << ") ";
        switch (e.type)
        {
        case Edge::kTree:
            std::cout << "Tree Edge\n";
            break;
        case Edge::kBack:
            std::cout << "Back Edge\n";
            break;
        case Edge::kForward:
            std::cout << "Forward Edge\n";
            break;
        case Edge::kCross:
            std::cout << "Cross Edge\n";
            break;
        }
    }
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

TEST_CASE("undirected")
{
    std::cout << "-------------------\n";
    Graph graph(6);
    graph.adj = {
        { 1, 3 },// 0
        { 0, 4, 3 },// 1
        { 5 },// 2
        { 0, 1, 4 },// 3
        { 1, 3 },// 4
        { 2, 5 }// 5
    };
    clock_t start = clock();
    std::list<Edge> result = DFSUndirected(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    for (Edge e : result)
    {
        std::cout << "(" << e.vertex_from << ", " << e.vertex_to << ") ";
        switch (e.type)
        {
        case Edge::kTree:
            std::cout << "Tree Edge\n";
            break;
        case Edge::kBack:
            std::cout << "Back Edge\n";
            break;
        case Edge::kForward:
            std::cout << "Forward Edge\n";
            break;
        case Edge::kCross:
            std::cout << "Cross Edge\n";
            break;
        }
    }
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
