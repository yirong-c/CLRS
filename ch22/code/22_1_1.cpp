#include <vector>
#include <list>

using AdjList = std::list<int>;

struct Graph
{
    std::vector< AdjList > adj;
};

std::vector<int> OutDegree(const Graph& graph)
{
    size_t v_size, i;
    v_size = graph.adj.size();
    std::vector<int> degree(v_size);
    for (i = 0; i < v_size; ++i)
    {
        // assume graph.adj[i].size() takes O(n) 
        // where n is size of  graph.adj[i]
        degree[i] = graph.adj[i].size();
    }
    return degree;
}

std::vector<int> InDegree(const Graph& graph)
{
    size_t v_size, i;
    v_size = graph.adj.size();
    std::vector<int> degree(v_size);
    for (i = 0; i < v_size; ++i)
    {
        for (int v : graph.adj[i])
        {
            ++degree[v];
        }
    }
    return degree;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("example")
{
    std::cout << "-------------------\n";
    Graph graph;
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
    std::vector<int> out_degree = OutDegree(graph);
    std::vector<int> in_degree = InDegree(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: \n";
    std::cout << "out_degree: ";
    for (int v : out_degree)
    {
        std::cout << v << " ";
    }
    std::cout << "\n";
    std::cout << "in_degree: ";
    for (int v : in_degree)
    {
        std::cout << v << " ";
    }
    std::cout << "\n";
    std::vector<int> solution_out_degree = { 0, 2, 1, 2, 1, 1, 1 };
    std::vector<int> solution_in_degree = { 0, 0, 2, 0, 2, 2, 2 };
    REQUIRE(out_degree == solution_out_degree);
    REQUIRE(in_degree == solution_in_degree);
}
