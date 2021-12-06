#include <limits.h>
#include <vector>
#include <list>
#include <queue>

using AdjList = std::list<int>;

enum Color { kWhite, kGray, kBlack };

struct Vertex
{
    Color color;
    std::list< std::pair<int, int> >::iterator result_pos;
};

struct Graph
{
    std::vector< AdjList > adj;
    std::vector< Vertex > vertices;

    Graph() {}

    Graph(int size) : adj(size), vertices(size) {}
};

std::list< std::pair<int, int> > TraverseEdge(Graph& graph, int src_idx)
{
    int u_idx, v_idx;
    std::list< std::pair<int, int> > traverse_result;
    std::list< std::pair<int, int> >::iterator result_it;
    std::queue<int> q;
    for (Vertex& u : graph.vertices)
    {
        u.color = Color::kWhite;
    }
    graph.vertices[src_idx].color = Color::kGray;
    graph.vertices[src_idx].result_pos = traverse_result.end();
    q.push(src_idx);
    while (q.empty() == false)
    {
        u_idx = q.front();
        q.pop();
        Vertex& u = graph.vertices[u_idx];
        result_it = u.result_pos;
        for (int v_idx : graph.adj[u_idx])
        {
            Vertex& v = graph.vertices[v_idx];
            if (v.color == Color::kWhite)
            {
                v.color = Color::kGray;
                traverse_result.emplace(result_it, u_idx, v_idx);
                v.result_pos = 
                    traverse_result.emplace(result_it, v_idx, u_idx);
                q.push(v_idx);
            }
            else if (v.color == Color::kGray)
            {
                traverse_result.emplace(result_it, u_idx, v_idx);
                traverse_result.emplace(result_it, v_idx, u_idx);
            }
        }
        u.color = Color::kBlack;
    }
    return traverse_result;
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
    std::list< std::pair<int, int> > result = TraverseEdge(graph, 1);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
    std::cout << "result: ";
    for (std::pair<int, int>& edge : result)
    {
        std::cout << "(" << edge.first << ", " << edge.second << ") ";
    }
    std::cout << "\n";
    std::list< std::pair<int, int> > solution = {
        { 1, 0 }, { 0, 4 }, { 4, 0 }, { 0, 1 }, { 1, 5 }, { 5, 2 },
        { 2, 3 }, { 3, 7 }, { 7, 3 }, { 3, 2 }, { 2, 6 }, { 6, 2 },
        { 2, 5 }, { 5, 6 }, { 6, 3 }, { 3, 6 }, { 6, 7 }, { 7, 6 },
        { 6, 5 }, { 5, 1 }
    };
    REQUIRE(result == solution);
}
