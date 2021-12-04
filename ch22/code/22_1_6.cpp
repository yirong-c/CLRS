#include <vector>

using Row = std::vector<bool>;
using Matrix = std::vector< Row >;

// graph must be a square matrix
// return vertex of universal sink
// return -1 if universal sink not exist
int UniversalSink(const Matrix& graph)
{
    size_t size, u, v;
    size = graph.size();
    // eliminate candidates
    u = 0;
    v = 1;
    while (v < size)
    {
        if (graph[u][v])
        {
            ++u;
            if (u == v)
            {
                ++v;
            }
        }
        else
        {
            ++v;
        }
    }
    // test the possible vertex u by claim 3
    for (v = 0; v < size; ++v)
    {
        if (graph[u][v])
            return -1;
    }
    for (v = 0; v < size; ++v)
    {
        if (graph[v][u] == false && u != v)
            return -1;
    }
    return u;
}

int UniversalSinkAnother(const Matrix& graph)
{
    size_t size, u, v;
    size = graph.size();
    u = 0;
    v = 0;
    while (u < size && v < size)
    {
        if (graph[u][v])
        {
            ++u;
        }
        else
        {
            ++v;
        }
    }
    if (u >= size)
        return -1;
    for (v = 0; v < size; ++v)
    {
        if (graph[u][v])
            return -1;
    }
    for (v = 0; v < size; ++v)
    {
        if (graph[v][u] == false && u != v)
            return -1;
    }
    return u;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

inline int GetRangedRandomNum(int left_end, int right_end)
{
	srand((unsigned)time(NULL));
	return rand() % (right_end - left_end + 1) + left_end;
}

TEST_CASE("example")
{
    std::cout << "-------------------\n";
    Matrix graph = {
        { 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0 },
        { 0, 0, 0, 0, 0, 1, 1 },
        { 0, 0, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 1 }
    };
    clock_t start = clock();
    int result = UniversalSink(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    REQUIRE(result == UniversalSinkAnother(graph));
    REQUIRE(result == -1);
}

TEST_CASE("2")
{
    std::cout << "-------------------\n";
    Matrix graph = {
        { 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 0 },
        { 1, 0, 0, 0, 1, 1, 1 },
        { 0, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 1, 0, 1 }
    };
    clock_t start = clock();
    int result = UniversalSink(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    REQUIRE(result == UniversalSinkAnother(graph));
    REQUIRE(result == 4);
}

TEST_CASE("random")
{
    std::cout << "-------------------\n";
    Matrix graph;
    clock_t start, end;
    int result_1, result_2;
    size_t i, u, v, size;
    unsigned long running_time_1, running_time_2;
    running_time_1 = 0;
    running_time_2 = 0;
    for (i = 0; i < 1000; ++i)
    {
        size = GetRangedRandomNum(50, 100);
        graph = Matrix(size, Row(size)); 
        for (u = 0; u < size; ++u)
        {
            for (v = 0; v < size; ++v)
            {
                graph[u][v] = GetRangedRandomNum(0, 1);
            }
        }

        start = clock();
        result_1 = UniversalSink(graph);
        end = clock();
        running_time_1 += (end - start);

        start = clock();
        result_2 = UniversalSinkAnother(graph);
        end = clock();
        running_time_2 += (end - start);
        
        REQUIRE(result_1 == result_2);
    }
    std::cout << "running time 1: " << 
        running_time_1 << " clocks" << std::endl;
    std::cout << "running time 1: " << 
        (double)(running_time_1) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
        std::cout << "running time 2: " << 
        running_time_2 << " clocks" << std::endl;
    std::cout << "running time 2: " << 
        (double)(running_time_2) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
}
