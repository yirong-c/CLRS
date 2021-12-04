#include <vector>
#include <list>

using AdjList = std::list<size_t>;

struct AdjListGraph
{
    std::vector< AdjList > adj;

    AdjListGraph() {}

    AdjListGraph(size_t size) : adj(size) {}
};

AdjListGraph Equivalent(const AdjListGraph& graph)
{
    size_t size, u;
    size = graph.adj.size();
    AdjListGraph target(size);
    std::vector<bool> edge_usage;
    for (u = 0; u < size; ++u)
    {
        edge_usage = std::vector<bool>(size, false);
        edge_usage[u] = true;
        for (int v : graph.adj[u])
        {
            if (edge_usage[v] == false)
            {
                target.adj[u].push_back(v);
                edge_usage[v] = true;
            }
        }
    }
    return target;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("example")
{
    std::cout << "-------------------\n";
    AdjListGraph graph;
    graph.adj = {
        {},
        { 2, 5, 2, 5 },
        { 1, 1, 2, 5, 3, 5, 4, 4 },
        { 2, 3, 4 },
        { 4, 2, 5, 5, 3 },
        { 4, 1, 2 }
    };
    clock_t start = clock();
    AdjListGraph result = Equivalent(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    AdjListGraph solution;
    solution.adj = {
        {},
        { 2, 5 },
        { 1, 5, 3, 4 },
        { 2, 4 },
        { 2, 5, 3 },
        { 4, 1, 2 },
    };
    REQUIRE(result.adj == solution.adj);
}
