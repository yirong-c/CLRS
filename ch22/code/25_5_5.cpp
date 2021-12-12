#include <limits.h>
#include <vector>
#include <list>

using AdjList = std::list<int>;

enum Color { kWhite, kGray, kBlack };

struct Vertex
{
    Color color;
    int prev;
    int discovery_time;
    int finishing_time;
    int component;
};

struct Graph
{
    int size;
    std::vector< AdjList > adj;
    std::vector< Vertex > vertices;
    std::vector< std::list<int> > vertices_lists_from_original_graph;

    Graph() {}

    Graph(int size) : size(size), adj(size), vertices(size),
        vertices_lists_from_original_graph(size) {}
};

void DFSVisit(Graph& graph, int u_idx, int& time, 
    std::list<int>& topo_sort)
{
    Vertex& u = graph.vertices[u_idx];
    ++time;
    u.discovery_time = time;
    u.color = Color::kGray;
    for (int v_idx : graph.adj[u_idx])
    {
        Vertex& v = graph.vertices[v_idx];
        if (v.color == Color::kWhite)
        {
            v.prev = u_idx;
            DFSVisit(graph, v_idx, time, topo_sort);
        }
    }
    u.color = Color::kBlack;
    ++time;
    u.finishing_time = time;
    topo_sort.push_front(u_idx);
}

std::list<int> TopologicalSort(Graph& graph)
{
    int time, u_idx;
    std::list<int> topo_sort;
    for (Vertex& u : graph.vertices)
    {
        u.color = Color::kWhite;
        u.prev = -1;
    }
    time = 0;
    for (u_idx = 0; u_idx < graph.size; ++u_idx)
    {
        if (graph.vertices[u_idx].color == Color::kWhite)
        {
            DFSVisit(graph, u_idx, time, topo_sort);
        }
    }
    return topo_sort;
}

Graph Transpose(const Graph& graph)
{
    size_t u;
    Graph target(graph.size);
    for (u = 0; u < graph.size; ++u)
    {
        for (int v : graph.adj[u])
        {
            target.adj[v].push_back(u);
        }
    }
    return target;
}

void DFSVisitComputeComponentGraph(Graph& graph, int u_idx, 
    std::list<int>& adj_list)
{
    Vertex& u = graph.vertices[u_idx];
    for (int v_idx : graph.adj[u_idx])
    {
        Vertex& v = graph.vertices[v_idx];
        if (v.component < 0)
        {
            v.component = u.component;
            DFSVisitComputeComponentGraph(graph, v_idx, adj_list);
        }
        else if (v.component != u.component)
        {
            adj_list.push_back(v.component);
        }
    }
}

std::list< std::list<int> > DFSComputeComponentGraphAdjList(Graph& graph, 
    const std::list<int>& order)
{
    int curr_component, v_idx;
    std::list< std::list<int> > adj_lists;
    for (Vertex& u : graph.vertices)
    {
        u.component = INT_MIN;
    }
    curr_component = 0;// index of component graph start from 0
    for (int u_idx : order)
    {
        Vertex& u = graph.vertices[u_idx];
        if (u.component < 0)
        {
            // DFS find edges (in adj_list) connected with curr_component
            u.component = curr_component;
            adj_lists.emplace_back(std::list<int>());
            DFSVisitComputeComponentGraph(graph, u_idx, adj_lists.back());
            ++curr_component;
        }
    }
    return adj_lists;
}

Graph ComponentGraphAdjLists(Graph& graph)
{
    int i;
    std::list<int> topo_sort = TopologicalSort(graph);
    Graph tranpose = Transpose(graph);
    std::list< std::list<int> > tranpose_component_graph_adj_lists = 
        DFSComputeComponentGraphAdjList(tranpose, topo_sort);
    // build ComponentGraph
    Graph component_graph(tranpose_component_graph_adj_lists.size());
    std::list< std::list<int> >::iterator adj_lists_it = 
        tranpose_component_graph_adj_lists.begin();
    for (i = 0; i < component_graph.size; ++i)
    {
        // adjacency list might exist repeat elements
        // we delete repeat elements by utilizing counter
        // Note that the adjacency list is for (G^T)^{SCC}
        // By Exercise 22.5-4, we showed that G^{SCC} = ((G^T)^{SCC})^T
        std::vector<bool> counter(i + 1, false);
        for (std::list<int>::iterator it = adj_lists_it->begin();
            it != adj_lists_it->end(); ++it)
        {
            if (counter[*it] == false)
            {
                counter[*it] = true;
                component_graph.adj[*it].push_back(i);
            }
        }
        ++adj_lists_it;
    }
    for (i = 0; i < graph.size; ++i)
    {
        component_graph.vertices_lists_from_original_graph
            [tranpose.vertices[i].component].push_back(i);
    }
    return component_graph;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("example 22.5-5")
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
    Graph component_graph = ComponentGraphAdjLists(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    Graph solution(4);
    solution.adj = { { 1, 2 }, { 2, 3 }, { 3 }, {  } };
    solution.vertices_lists_from_original_graph = 
        { { 0, 1, 4 }, { 2, 3 }, { 5, 6 }, { 7 } };
    REQUIRE(component_graph.size == solution.size);
    REQUIRE(component_graph.adj == solution.adj);
    REQUIRE(component_graph.vertices_lists_from_original_graph 
        == solution.vertices_lists_from_original_graph);
}
