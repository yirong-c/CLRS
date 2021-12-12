#include "25_5_5.cpp"

bool Semiconnected(Graph& graph)
{
    int u;
    Graph component_graph = ComponentGraphAdjLists(graph);
    std::list<int> topo_sort = TopologicalSort(component_graph);
    std::list<int>::iterator it = topo_sort.begin();
    std::list<int>::iterator end = topo_sort.end();
    u = *it;
    ++it;
    while (it != end)
    {
        if (std::find(component_graph.adj[u].begin(), component_graph.adj[u].end(), 
            *it) == component_graph.adj[u].end())
            return false;
        u = *it;
    }
    return true;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("example 22.5-7")
{
    std::cout << "-------------------\n";
    Graph graph(8);
    graph.adj = {
        { 1 },// 0
        { 2, 4, 5 },// 1
        { 3, 6 },// 2
        { 2, 7 },// 3
        { 0, 5 },// 4
        { 6 },// 5
        { 5, 7 },// 6
        { 7 }// 7
    };
    clock_t start = clock();
    bool result = Semiconnected(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    REQUIRE(result == false);
}
