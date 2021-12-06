#include <limits.h>
#include <vector>
#include <list>
#include <queue>

using AdjList = std::list<int>;

enum Color { kWhite, kGray, kBlack };

struct Vertex
{
    Color color;
    int distance;
    int prev;
};

struct Graph
{
    std::vector< AdjList > adj;
    std::vector< Vertex > vertices;

    Graph() {}

    Graph(int size) : adj(size), vertices(size) {}
};

std::list<int> BFS(Graph& graph, int src_idx)
{
    int u_idx, v_idx;
    std::list<int> discover_result;
    std::queue<int> q;
    for (Vertex& u : graph.vertices)
    {
        u.color = Color::kWhite;
        u.distance = INT_MAX;
        u.prev = -1;
    }
    graph.vertices[src_idx].color = Color::kGray;
    graph.vertices[src_idx].distance = 0;
    graph.vertices[src_idx].prev = -1;
    q.push(src_idx);
    while (q.empty() == false)
    {
        u_idx = q.front();
        q.pop();
        discover_result.push_back(u_idx);
        Vertex& u = graph.vertices[u_idx];
        for (int v_idx : graph.adj[u_idx])
        {
            Vertex& v = graph.vertices[v_idx];
            if (v.color == Color::kWhite)
            {
                v.color = Color::kGray;
                v.distance = u.distance + 1;
                v.prev = u_idx;
                q.push(v_idx);
            }
        }
        u.color = Color::kBlack;
    }
    return discover_result;
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
        { 1, 4 },
        { 0, 5 },
        { 3, 5, 6 },
        { 2, 6, 7 },
        { 0 },
        { 1, 2, 6 },
        { 2, 3, 5, 7 },
        { 3, 6 }
    };
    clock_t start = clock();
    std::list<int> result = BFS(graph, 1);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    std::list<int> solution = { 1, 0, 5, 4, 2, 6, 3, 7 };
    REQUIRE(result == solution);
}
