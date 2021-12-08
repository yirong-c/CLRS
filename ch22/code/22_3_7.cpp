#include <vector>
#include <list>
#include <stack>

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

struct StackElement
{
    int vertex_idx;
    AdjList::iterator adj_it_curr;
    AdjList::iterator adj_it_end;
};

void DFS(Graph& graph)
{
    int time, u_idx, v_idx;
    std::stack< StackElement > path;
    AdjList vertices_list;
    for (u_idx = 0; u_idx < graph.size; ++u_idx)
    {
        Vertex& u = graph.vertices[u_idx];
        u.color = Color::kWhite;
        vertices_list.push_back(u_idx);
    }
    time = 0;
    path.push({ -1, vertices_list.begin(), vertices_list.end() });
    while (true)
    {
        u_idx = path.top().vertex_idx;
        AdjList::iterator& v_it = path.top().adj_it_curr;
        if (v_it == path.top().adj_it_end)
        {
            path.pop();
            if (path.empty())
                break;
            Vertex& u = graph.vertices[u_idx];
            u.color = Color::kBlack;
            ++time;
            u.finishing_time = time;
        }
        else
        {
            v_idx = *v_it;
            Vertex& v = graph.vertices[v_idx];
            if (v.color == Color::kWhite)
            {
                ++time;
                v.discovery_time = time;
                v.color = Color::kGray;
                v.prev = u_idx;
                path.push({ v_idx, graph.adj[v_idx].begin(), graph.adj[v_idx].end() });
            }
            ++v_it;
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
