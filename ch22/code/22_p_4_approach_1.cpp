#include <algorithm>
#include <climits>
#include <list>
#include <vector>

enum Color { kWhite, kNonWhite };

struct OriginalVertex
{
    Color color;
    int value;// L
    int min;
};

struct TranposeVertex
{
    Color color;
    int component = INT_MIN;
};

struct SCCVertex
{
    Color color;
    std::list<int> vertices;
    int dp_min_scc_vertex;
    int dp_min_scc_value;
};

template <typename VertexType>
struct Graph
{
    int size;
    std::vector< std::list<int> > adj;
    std::vector< VertexType > vertices;
    
    Graph(int size) : size(size), adj(size), vertices(size) {}

    void WhitenAllVertices()
    {
        for (VertexType u : vertices)
        {
            u.color = Color::kWhite;
        }
    }
};

Graph<TranposeVertex> Tranpose(const Graph<OriginalVertex>& graph)
{
    int u;
    Graph<TranposeVertex> result(graph.size);
    for (u = 0; u < graph.size; ++u)
    {
        for (int v : graph.adj[u])
        {
            result.adj[v].push_back(u);
        }
    }
    return result;
}

void TopoSortVisit(Graph<OriginalVertex>& graph, int u, 
    std::list<int>& topo_sort_result)
{
    graph.vertices[u].color = Color::kNonWhite;
    for (int v : graph.adj[u])
    {
        if (graph.vertices[v].color == Color::kWhite)
            TopoSortVisit(graph, v, topo_sort_result);
    }
    topo_sort_result.push_front(u);
}

// component_adj_list is for tranpose graph and might exist repeat elements 
void SCCTranposeVisit(Graph<TranposeVertex>& tranpose, int u, 
    std::list<int>& component_vertices, std::list<int>& component_adj_list, 
    int curr_component)
{
    tranpose.vertices[u].color = Color::kNonWhite;
    tranpose.vertices[u].component = curr_component;
    component_vertices.push_back(u);
    for (int v : tranpose.adj[u])
    {
        if (tranpose.vertices[v].color == Color::kWhite)
            SCCTranposeVisit(tranpose, v, component_vertices, 
                component_adj_list, curr_component);
        else if (tranpose.vertices[v].component != curr_component)
            // default value of component attribute is INT_MIN
            component_adj_list.push_back(tranpose.vertices[v].component);
    }
}

void DpSCCVisit(Graph<SCCVertex>& scc, int u)
{
    scc.vertices[u].color = Color::kNonWhite;
    for (int v : scc.adj[u])
    {
        if (scc.vertices[v].color == Color::kWhite)
        {
            DpSCCVisit(scc, v);
            if (scc.vertices[u].dp_min_scc_value > scc.vertices[v].dp_min_scc_value)
            {
                scc.vertices[u].dp_min_scc_vertex = scc.vertices[v].dp_min_scc_vertex;
                scc.vertices[u].dp_min_scc_value = scc.vertices[v].dp_min_scc_value;
            }
        }
    }
}

void Reachability(Graph<OriginalVertex>& graph)
{
    int u, component_counter;
    std::list<int> topo_sort_result;
    std::list< std::list<int> > component_vertices_collection, 
        tranpose_component_adj_list_collection;
    // topo sort
    graph.WhitenAllVertices();
    for (u = 0; u < graph.size; ++u)
    {
        if (graph.vertices[u].color == Color::kWhite)
            TopoSortVisit(graph, u, topo_sort_result);
    }
    // find tranpose
    Graph<TranposeVertex> tranpose = Tranpose(graph);
    // find strongly connected components
    tranpose.WhitenAllVertices();
    component_counter = 0;
    for (int w : topo_sort_result)
    {
        if (tranpose.vertices[w].color == Color::kWhite)
        {
            component_vertices_collection.emplace_back();
            tranpose_component_adj_list_collection.emplace_back();
            SCCTranposeVisit(tranpose, w, component_vertices_collection.back(), 
                tranpose_component_adj_list_collection.back(), component_counter);
            ++component_counter;
        }
    }
    // make scc graph
    Graph<SCCVertex> scc(component_counter);
    u = 0;
    for (std::list<int>& component_vertices : component_vertices_collection)
    {
        scc.vertices[u].vertices.assign(component_vertices.begin(), 
            component_vertices.end());
        ++u;
    }
    u = 0;
    for (std::list<int>& component_adj_list : tranpose_component_adj_list_collection)
    {
        std::vector<bool> repeat_counter(scc.size, false);
        for (int v : component_adj_list)
        {
            if (repeat_counter[v] == false)
            {
                scc.adj[v].push_back(u);
                repeat_counter[v] = true;
            }
        }
        ++u;
    }
    // init dp_min_scc_vertex and dp_min_scc_value for each scc vertex
    for (SCCVertex& scc_v : scc.vertices)
    {
        scc_v.dp_min_scc_vertex = *(std::min_element(scc_v.vertices.begin(), 
            scc_v.vertices.end(), [&graph](int a, int b) { 
                return graph.vertices[a].value < graph.vertices[b].value; 
            }));
        scc_v.dp_min_scc_value = graph.vertices[scc_v.dp_min_scc_vertex].value;
    }
    // dp find min for each scc
    scc.WhitenAllVertices();
    for (u = 0; u < scc.size; ++u)
    {
        if (scc.vertices[u].color == Color::kWhite)
            DpSCCVisit(scc, u);
    }
    // set min attribute for each original vertex
    for (SCCVertex& scc_v : scc.vertices)
    {
        for (int v : scc_v.vertices)
        {
            graph.vertices[v].min = scc_v.dp_min_scc_vertex;
        }
    }
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("example")
{
    std::cout << "-------------------\n";
    Graph<OriginalVertex> graph(8);
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
    graph.vertices[0].value = 3;
    graph.vertices[1].value = 6;
    graph.vertices[2].value = 7;
    graph.vertices[3].value = 1;
    graph.vertices[4].value = 0;
    graph.vertices[5].value = 4;
    graph.vertices[6].value = 2;
    graph.vertices[7].value = 5;
    clock_t start = clock();
    Reachability(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    REQUIRE(graph.vertices[0].min == 4);
    REQUIRE(graph.vertices[1].min == 4);
    REQUIRE(graph.vertices[2].min == 3);
    REQUIRE(graph.vertices[3].min == 3);
    REQUIRE(graph.vertices[4].min == 4);
    REQUIRE(graph.vertices[5].min == 6);
    REQUIRE(graph.vertices[6].min == 6);
    REQUIRE(graph.vertices[7].min == 7);
}
