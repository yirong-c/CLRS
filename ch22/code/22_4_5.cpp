#include <vector>
#include <list>
#include <queue>

using AdjList = std::list<int>;

struct Graph
{
    int size;
    std::vector< AdjList > adj;
    
    Graph() {}

    Graph(int size) : size(size), adj(size) {}
};

std::list<int> TopologicalSort(Graph& graph)
{
    int u;
    std::list<int> topo_sort;
    std::vector<int> in_degree_counter(graph.size, 0);
    std::queue<int> zero_in_degree;
    // compute in-degree
    for (AdjList& adj_list : graph.adj)
    {
        for (int v : adj_list)
        {
            ++in_degree_counter[v];
        }
    }
    // init queue contains vertices with zero in-degree
    for (u = 0; u < graph.size; ++u)
    {
        if (in_degree_counter[u] == 0)
        {
            zero_in_degree.push(u);
        }
    }
    // perform topological sort
    while (zero_in_degree.empty() == false)
    {
        u = zero_in_degree.front();
        zero_in_degree.pop();
        topo_sort.push_back(u);
        for (int v : graph.adj[u])
        {
            --in_degree_counter[v];
            if (in_degree_counter[v] == 0)
            {
                zero_in_degree.push(v);
            }
        }
    }
    return topo_sort;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("example")
{
    std::cout << "-------------------\n";
    Graph graph(9);
    graph.adj = {
        { 2, 3 },// 0
        { 3 },// 1
        { 3, 4 },// 2
        {},// 3
        { 7 },// 4
        { 4, 6 },// 5
        { 7 },// 6
        {},// 7
        {}// 8
    };
    clock_t start = clock();
    std::list<int> topo_sort = TopologicalSort(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    std::list<int> solution = { 0, 1, 5, 8, 2, 6, 3, 4, 7 };
    REQUIRE(topo_sort == solution);
}
