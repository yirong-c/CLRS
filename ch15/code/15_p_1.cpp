#include <vector>
#include <list>
#include <limits.h>

struct AdjListElement
{
    int vertex;
    int weight;
};

using AdjList = std::list<AdjListElement>;
using Graph = std::vector<AdjList>;

using Row = std::vector<int>;

// O(E)
int RecursiveLongestSimplePath(const Graph& graph, int start, int dist, 
    Row& length, Row& next)
{
    int curr_length;
    if (length[start] >= 0)
        return length[start];
    for (const AdjListElement& adj_element : graph[start])
    {
        curr_length = adj_element.weight + RecursiveLongestSimplePath(graph, 
            adj_element.vertex, dist, length, next);
        if (curr_length > length[start])
        {
            length[start] = curr_length;
            next[start] = adj_element.vertex;
        }
    }
    return length[start];
}

// O(V + E)
std::list<int> LongestSimplePath(const Graph& graph, int start, int dist)
{
    int vertex;
    size_t size;
    std::list<int> result;
    size = graph.size();
    Row length(size, INT_MIN), next(size);
    length[dist] = 0;
    RecursiveLongestSimplePath(graph, start, dist, length, next);
    if (length[start] < 0)
        return result;
    vertex = start;
    while (vertex != dist)
    {
        result.push_back(vertex);
        vertex = next[vertex];
    }
    result.push_back(dist);
    return result;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>

TEST_CASE()
{
    std::list<int> result, solution;
    Graph graph(4);
    graph[0] = {
        { 1, 10 },
        { 2, 5 }
    };
    graph[1] = {
        { 3, 30 }
    };
    graph[2] = {
        { 1, 20 },
        { 3, 40 }
    };
    graph[3] = {};
    result = LongestSimplePath(graph, 0, 3);
    solution = { 0, 2, 1, 3 };
    REQUIRE(result == solution);
    result = LongestSimplePath(graph, 3, 0);
    solution = {};
    REQUIRE(result == solution);
    result = LongestSimplePath(graph, 2, 1);
    solution = { 2, 1 };
    REQUIRE(result == solution);
    result = LongestSimplePath(graph, 2, 3);
    solution = { 2, 1, 3 };
    REQUIRE(result == solution);
    result = LongestSimplePath(graph, 1, 2);
    solution = {};
    REQUIRE(result == solution);
    result = LongestSimplePath(graph, 0, 0);
    solution = { 0 };
    REQUIRE(result == solution);
}
