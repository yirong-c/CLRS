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

// graph is strongly connected, directed
std::list< std::pair<int, int> > EulerTour(Graph& graph)
{
    int u, v, s;

    if (graph.size <= 1)
        return std::list< std::pair<int, int> >();

    std::vector< AdjList > adjs_copy = graph.adj;// make a copy of adjacency list
    std::list< std::pair<int, int> > tour;// result
    std::vector< std::list< std::pair<int, int> >::iterator > 
        tour_its(graph.size, tour.end());// iterators of tours for each insert-availble 
                                         // (in the tour list) vertices
    std::queue<int> todo;// queue of vertices in the tour list already
    
    todo.push(0);

    while (todo.empty() == false)
    {
        u = todo.front();
        s = u;
        todo.pop();
        std::list< std::pair<int, int> >::iterator tour_curr_it = tour_its[u];
            // current iterator of insert postion in the tour list

        while (adjs_copy[u].empty() == false)
        {
            v = adjs_copy[u].front();
            tour_its[u] = tour.insert(tour_curr_it, { u, v });
            adjs_copy[u].pop_front();
            todo.push(u);
            u = v;
        }

        if (s != u)
            return std::list< std::pair<int, int> >();
    }

    return tour;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("no merge")
{
    std::cout << "-------------------\n";
    Graph graph(4);
    graph.adj = {
        { 1 },// 0
        { 2, 0 },// 1
        { 3 },// 2
        { 1 },// 3
    };
    clock_t start = clock();
    std::list< std::pair<int, int> > result = EulerTour(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    std::list< std::pair<int, int> > solution = {
        { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 1 }, { 1, 0 }
    };
    REQUIRE(result == solution);
}

TEST_CASE("one merge")
{
    std::cout << "-------------------\n";
    Graph graph(4);
    graph.adj = {
        { 1 },// 0
        { 0, 2 },// 1
        { 3 },// 2
        { 1 },// 3
    };
    clock_t start = clock();
    std::list< std::pair<int, int> > result = EulerTour(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    std::list< std::pair<int, int> > solution = {
        { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 1 }, { 1, 0 }
    };
    REQUIRE(result == solution);
}

TEST_CASE("three merge")
{
    std::cout << "-------------------\n";
    Graph graph(8);
    graph.adj = {
        { 1, 7 },// 0
        { 0, 2 },// 1
        { 3, 7 },// 2
        { 0, 2, 4 },// 3
        { 3, 5 },// 4
        { 6 },// 5
        { 4 },// 6
        { 1, 3 },// 7
    };
    clock_t start = clock();
    std::list< std::pair<int, int> > result = EulerTour(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    std::list< std::pair<int, int> > solution = {
        { 0, 1 }, { 1, 0 }, { 0, 7 }, { 7, 3 }, { 3, 4 }, 
        { 4, 5 }, { 5, 6 }, { 6, 4 }, { 4, 3 }, { 3, 2 }, 
        { 2, 7 }, { 7, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 }, 
    };
    REQUIRE(result == solution);
}

TEST_CASE("no tour 1")
{
    std::cout << "-------------------\n";
    Graph graph(8);
    graph.adj = {
        { 1 },// 0
        { 0, 2 },// 1
        { 3, 7 },// 2
        { 0, 2, 4 },// 3
        { 3, 5 },// 4
        { 6 },// 5
        { 4 },// 6
        { 3 },// 7
    };
    clock_t start = clock();
    std::list< std::pair<int, int> > result = EulerTour(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    std::list< std::pair<int, int> > solution = { };
    REQUIRE(result == solution);
}

TEST_CASE("no tour 2")
{
    std::cout << "-------------------\n";
    Graph graph(8);
    graph.adj = {
        { 1 },// 0
        { 0, 2 },// 1
        { 3 },// 2
        { 0, 2, 4 },// 3
        { 3, 5 },// 4
        { 6 },// 5
        { 4 },// 6
    };
    clock_t start = clock();
    std::list< std::pair<int, int> > result = EulerTour(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    std::list< std::pair<int, int> > solution = { };
    REQUIRE(result == solution);
}

TEST_CASE("no tour")
{
    std::cout << "-------------------\n";
    Graph graph(4);
    graph.adj = {
        { 1 },// 0
        { 0, 2, 3 },// 1
        { 1, 3 },// 2
        { 1 },// 3
    };
    clock_t start = clock();
    std::list< std::pair<int, int> > result = EulerTour(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    std::list< std::pair<int, int> > solution = { };
    REQUIRE(result == solution);
}

TEST_CASE("no tour 4")
{
    std::cout << "-------------------\n";
    Graph graph(4);
    graph.adj = {
        { 1 },// 0
        { 2 },// 1
        { 3 },// 2
        { 1 },// 3
    };
    clock_t start = clock();
    std::list< std::pair<int, int> > result = EulerTour(graph);
    clock_t end = clock();
    std::cout << "running time: " << 
        (end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "\n";
    std::list< std::pair<int, int> > solution = { };
    REQUIRE(result == solution);
}
