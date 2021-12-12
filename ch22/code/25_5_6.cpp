#include "25_5_5.cpp"

Graph CreateGraphExercise22_5_6(Graph& graph)
{
    int component_idx, u;
    Graph component_graph = ComponentGraphAdjLists(graph);
    Graph result_graph(graph.size);
    for (component_idx = 0; component_idx < component_graph.size; ++component_idx)
    {
        // build cycle
        std::list<int>& vertices = 
            component_graph.vertices_lists_from_original_graph[component_idx];
        if (vertices.front() != vertices.back())
        {
            u = vertices.back();
            for (int v : vertices)
            {
                result_graph.adj[u].push_back(v);
                u = v;
            }
        }
        // build edges among components
        u = vertices.front();
        std::list<int>& adj = component_graph.adj[component_idx];
        for (int c : adj)
        {
            int v = component_graph.vertices_lists_from_original_graph[c].front();
            result_graph.adj[u].push_back(v);
        }
    }
    return result_graph;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("example 22.5-6")
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
    Graph result = CreateGraphExercise22_5_6(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    Graph solution(8);
    solution.adj = { { 1, 2, 5 }, { 4 }, { 3, 5, 7 }, { 2 }, { 0 }, { 6, 7 }, { 5 }, {  } };
    REQUIRE(result.size == solution.size);
    REQUIRE(result.adj == solution.adj);
}
