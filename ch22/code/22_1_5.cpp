#include <vector>
#include <list>

#include "../../ch4/matrix/strassen.hpp"

using AdjList = std::list<size_t>;

struct AdjListGraph
{
    std::vector< AdjList > adj;

    AdjListGraph() {}

    AdjListGraph(size_t size) : adj(size) {}
};

using AdjMatrixGraph = UniqueMatrix<int>;

// struct AdjMatrixGraph
// {
//     UniqueMatrix<int> adj;

//     AdjMatrixGraph(size_t size) : 
//         adj(std::vector< std::vector<int> >(size, std::vector<int>(size))) {}
// };

AdjListGraph Square(const AdjListGraph& graph)
{
    size_t size, u;
    size = graph.adj.size();
    AdjListGraph result(size);
    for (u = 0; u < size; ++u)
    {
        for (int v : graph.adj[u])
        {
            result.adj[u].push_back(v);
            for (int w : graph.adj[v])
            {
                result.adj[u].push_back(w);
            }
        }
    }
    return result;
}

AdjMatrixGraph Square(const AdjMatrixGraph& graph)
{
    size_t size, u, v, w;
    size = graph.Rows();
    AdjMatrixGraph result(size, size);
    for (u = 0; u < size; ++u)
    {
        for (v = 0; v < size; ++v)
        {
            if (graph[u][v])
            {
                result[u][v] = true;
                for (w = 0; w < size; ++w)
                {
                    if (graph[v][w])
                    {
                        result[u][w] = true;
                    }
                }
            }
        }
    }
    return result;
}

AdjMatrixGraph SquareByStrassen(const AdjMatrixGraph& graph)
{
    size_t size, u, v, w;
    size = graph.Rows();
    AdjMatrixGraph result = StrassenMultiplication(graph, graph);
    for (u = 0; u < size; ++u)
    {
        for (v = 0; v < size; ++v)
        {
            if (graph[u][v])
            {
                result[u][v] = 1;
            }
        }
    }
    return result;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

bool CheckAdjListGraphSimpleEquiv(const AdjListGraph& first, 
    const AdjListGraph& second)
{
    size_t u, size;
    size = first.adj.size();
    assert(size == second.adj.size());
    std::vector<bool> first_edges, second_edges; 
    for (u = 0; u < size; ++u)
    {
        first_edges = std::vector<bool>(size);
        second_edges = std::vector<bool>(size);
        for (int v : first.adj[u])
        {
            first_edges[v] = true;
        }
        for (int v : second.adj[u])
        {
            second_edges[v] = true;
        }
        assert(first_edges == second_edges);
    }
    return true;
}

bool CheckAdjMatrixGraphSimpleEquiv(const AdjMatrixGraph& first, 
    const AdjMatrixGraph& second)
{
    size_t u, v, size;
    size = first.Rows();
    assert(size == second.Rows());
    assert(size == first.Cols());
    assert(first.Cols() == second.Cols());
    std::vector<bool> first_edges, second_edges; 
    for (u = 0; u < size; ++u)
    {
        for (v = 0; v < size; ++v)
        {
            assert((bool)(first[u][v]) == (bool)(second[u][v]));
        }
    }
    return true;
}

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
    AdjListGraph result = Square(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    AdjListGraph solution;
    solution.adj = {
        {},
        { 2, 4, 5 },
        { 5, 4 },
        { 6, 5, 4 },
        { 2, 5 },
        { 4, 2 },
        { 6 }
    };
    REQUIRE(CheckAdjListGraphSimpleEquiv(result, solution));
}

TEST_CASE("example AdjMatrixGraph")
{
    std::cout << "-------------------\n";
    AdjMatrixGraph graph = {
        { 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0 },
        { 0, 0, 0, 0, 0, 1, 1 },
        { 0, 0, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 1 }
    };
    clock_t start = clock();
    AdjMatrixGraph result = Square(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    AdjMatrixGraph solution = {
        { 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 1, 1, 0 },
        { 0, 0, 0, 0, 1, 1, 0 },
        { 0, 0, 0, 0, 1, 1, 1 },
        { 0, 0, 1, 0, 0, 1, 0 },
        { 0, 0, 1, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 1 }
    };
    REQUIRE(CheckAdjMatrixGraphSimpleEquiv(result, solution));
}

TEST_CASE("example SquareByStrassen")
{
    std::cout << "-------------------\n";
    AdjMatrixGraph graph = {
        { 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0 },
        { 0, 0, 0, 0, 0, 1, 1 },
        { 0, 0, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 1 }
    };
    graph.ResizeRow(8);
    graph.ResizeCol(8);
    clock_t start = clock();
    AdjMatrixGraph result = SquareByStrassen(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    AdjMatrixGraph solution = {
        { 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 1, 1, 0 },
        { 0, 0, 0, 0, 1, 1, 0 },
        { 0, 0, 0, 0, 1, 1, 1 },
        { 0, 0, 1, 0, 0, 1, 0 },
        { 0, 0, 1, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 1 }
    };
    solution.ResizeRow(8);
    solution.ResizeCol(8);
    REQUIRE(CheckAdjMatrixGraphSimpleEquiv(result, solution));
}
