#include <limits.h>
#include <vector>
#include <list>
#include <queue>

using AdjList = std::list<int>;

enum Color { kWhite, kNonWhite };
enum WrestlerType { kBabyfaces, kHeels };

struct Vertex
{
    Color color;
    WrestlerType type;
};

struct Graph
{
    std::vector< AdjList > adj;
    std::vector< Vertex > vertices;

    Graph() {}

    Graph(int size) : adj(size), vertices(size) {}
};

bool DesignateWrestlerType(Graph& graph)
{
    int u_idx, v_idx;
    std::queue<int> q;
    for (Vertex& u : graph.vertices)
    {
        u.color = Color::kWhite;
    }
    graph.vertices[0].color = Color::kNonWhite;
    graph.vertices[0].type = WrestlerType::kBabyfaces;
    q.push(0);
    while (q.empty() == false)
    {
        u_idx = q.front();
        q.pop();
        Vertex& u = graph.vertices[u_idx];
        for (int v_idx : graph.adj[u_idx])
        {
            Vertex& v = graph.vertices[v_idx];
            if (v.color == Color::kWhite)
            {
                v.color = Color::kNonWhite;
                graph.vertices[v_idx].type = (u.type == WrestlerType::kBabyfaces) ? 
                    WrestlerType::kHeels : WrestlerType::kBabyfaces;
                q.push(v_idx);
            }
            else
            {
                if (u.type == v.type)
                    return false;
            }
        }
    }
    return true;
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
    bool result = DesignateWrestlerType(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    REQUIRE(result == false);
}
