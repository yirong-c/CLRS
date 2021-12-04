#include <vector>
#include <list>

using AdjList = std::list<size_t>;

struct AdjListGraph
{
    std::vector< AdjList > adj;

    AdjListGraph() {}

    AdjListGraph(size_t size) : adj(size) {}
};

struct AdjMatrixGraph
{
    std::vector< std::vector<bool> > adj;

    AdjMatrixGraph() {}

    AdjMatrixGraph(size_t size) : adj(size, std::vector<bool>(size)) {}
};

AdjListGraph Transpose(const AdjListGraph& graph)
{
    size_t size, u;
    size = graph.adj.size();
    AdjListGraph target(size);
    for (u = 0; u < size; ++u)
    {
        for (int v : graph.adj[u])
        {
            target.adj[v].push_back(u);
        }
    }
    return target;
}

AdjMatrixGraph Transpose(const AdjMatrixGraph& graph)
{
    size_t size, u, v;
    size = graph.adj.size();
    AdjMatrixGraph target(size);
    for (u = 0; u < size; ++u)
    {
        for (v = 0; v < size; ++v)
        {
            target.adj[v][u] = graph.adj[u][v];
        }
    }
    return target;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("example AdjListGraph")
{
    std::cout << "-------------------\n";
    AdjListGraph graph;
    graph.adj = {
        {},
        { 2, 4 },
        { 5 },
        { 6, 5 },
        { 2 },
        { 4 },
        { 6 }
    };
    clock_t start = clock();
    AdjListGraph result = Transpose(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    AdjListGraph solution;
    solution.adj = {
        {},
        {},
        { 1, 4 },
        {},
        { 1, 5 },
        { 2, 3 },
        { 3, 6 }
    };
    REQUIRE(result.adj == solution.adj);
}

TEST_CASE("example AdjMatrixGraph")
{
    std::cout << "-------------------\n";
    AdjMatrixGraph graph;
    graph.adj = {
        { 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0 },
        { 0, 0, 0, 0, 0, 1, 1 },
        { 0, 0, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 1 }
    };
    clock_t start = clock();
    AdjMatrixGraph result = Transpose(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    AdjMatrixGraph solution;
    solution.adj = {
        { 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 1, 0 },
        { 0, 0, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 1, 0, 0, 1 }
    };
    REQUIRE(result.adj == solution.adj);
}
