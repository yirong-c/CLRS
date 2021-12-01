#include <vector>
#include <list>
#include <limits.h>

using Sound = int;

struct AdjListElement
{
    int vertex_index;
    Sound sound;
    double probability;
};

struct Vertex
{
    std::list<AdjListElement> adj_list;
};

using Graph = std::vector<Vertex>;

bool ViterbiAux(Graph& graph, int v, 
    const std::vector<Sound>& sigma, 
    int sigma_index, std::list<int>& result, 
    std::vector< std::vector<bool> >& dp_checked)
{
    bool exist_path;
    if (sigma_index >= sigma.size())
        return true;
    if (dp_checked[v][sigma_index])// dp
        return false;
    dp_checked[v][sigma_index] = true;
    for (const AdjListElement& edge : graph[v].adj_list)
    {
        if (sigma[sigma_index] == edge.sound)
        {
            if (ViterbiAux(graph, edge.vertex_index, sigma, 
                sigma_index + 1, result, dp_checked))
            {
                result.push_front(edge.vertex_index);
                return true;
            }
        }
    }
    return false;
}

// index of sigma start from 0
// return empty list if NO-SUCH-PATH
std::list<int> Viterbi(Graph& graph, int v0, 
    const std::vector<int>& sigma)
{
    std::list<int> result;
    std::vector< std::vector<bool> > dp_checked(graph.size(), 
        std::vector<bool>(sigma.size(), false));
    if (ViterbiAux(graph, v0, sigma, 0, result, dp_checked))
        result.push_front(v0);
    return result;
}

double ViterbiProbabilityAux(Graph& graph, int v, 
    const std::vector<Sound>& sigma, 
    int sigma_index, std::vector< std::vector<int> >& dp_path, 
    std::vector< std::vector<double> >& dp_probability)
{
    double l_probability;
    if (sigma_index >= sigma.size())
        return 1;
    if (dp_probability[v][sigma_index] >= 0)// dp
        return dp_probability[v][sigma_index];
    dp_probability[v][sigma_index] = 0;
    for (const AdjListElement& edge : graph[v].adj_list)
    {
        if (sigma[sigma_index] == edge.sound)
        {
            l_probability = ViterbiProbabilityAux(graph, edge.vertex_index, 
                sigma, sigma_index + 1, dp_path, dp_probability)
                * edge.probability;
            if (l_probability > dp_probability[v][sigma_index])
            {
                dp_probability[v][sigma_index] = l_probability;
                dp_path[v][sigma_index] = edge.vertex_index;
            }
        }
    }
    return dp_probability[v][sigma_index];
}

// index of sigma start from 0
// return empty list if NO-SUCH-PATH
std::list<int> ViterbiProbability(Graph& graph, int v0, 
    const std::vector<int>& sigma)
{
    int l_vertex;
    size_t vertex_size, sigma_size, i;
    std::list<int> result;
    vertex_size = graph.size();
    sigma_size = sigma.size();
    std::vector< std::vector<int> > dp_path(vertex_size, 
        std::vector<int>(sigma_size));
    std::vector< std::vector<double> > dp_probability(vertex_size, 
        std::vector<double>(sigma_size, -1));
    if (ViterbiProbabilityAux(graph, v0, sigma, 0, dp_path, 
        dp_probability) > 0)
    {
        l_vertex = v0;
        result.push_back(l_vertex);
        for (i = 0; i < sigma_size; ++i)
        {
            l_vertex = dp_path[l_vertex][i];
            result.push_back(l_vertex);
        }
    }
    return result;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("(a) 1")
{
    std::cout << "------------------------" << std::endl;
    Graph graph = {
        { {} },
        { { { 2, 30, 0 }, { 3, 40, 0 }, { 5, 30, 0 } } },
        { { { 6, 10, 0 }, { 7, 10, 0 } } },
        { { { 2, 10, 0 }, { 4, 20, 0 } } },
        { { { 2, 40, 0 }, { 5, 50, 0 } } },
        { { { 3, 40, 0 } } },
        { { { 4, 20, 0 } } },
        { { { 1, 20, 0 }, { 6, 30, 0 } } },
    };
    std::vector<int> sigma = { 10, 20, 30, 40 };
    clock_t start = clock();
    std::list<int> result = Viterbi(graph, 2, sigma);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: ";
    for (int vertex : result)
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    std::list<int> solution = { 2, 7, 1, 5, 3 };
    REQUIRE(result == solution);
}

TEST_CASE("(a) 2")
{
    std::cout << "------------------------" << std::endl;
    Graph graph = {
        { {}  },
        { { { 2, 30, 0 }, { 3, 40, 0 }, { 5, 50, 0 } } },
        { { { 6, 10, 0 }, { 7, 10, 0 } } },
        { { { 2, 10, 0 }, { 4, 20, 0 } } },
        { { { 2, 40, 0 }, { 5, 50, 0 } } },
        { { { 3, 40, 0 } } },
        { { { 4, 20, 0 } } },
        { { { 1, 20, 0 }, { 6, 30, 0 } } },
    };
    std::vector<int> sigma = { 10, 20, 30, 40 };
    clock_t start = clock();
    std::list<int> result = Viterbi(graph, 2, sigma);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: ";
    for (int vertex : result)
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    std::list<int> solution = {};
    REQUIRE(result == solution);
}

TEST_CASE("(a) 3")
{
    std::cout << "------------------------" << std::endl;
    Graph graph = {
        { {} },
        { { { 2, 10, 0 }, { 3, 10, 0 } } },
        { { { 4, 20, 0 } } },
        { { { 4, 20, 0 } } },
        { { { 5, 30, 0 } } },
        { { { 3, 40, 0 } } }
    };
    std::vector<int> sigma = { 10, 20, 30, 40, 20, 50 };
    clock_t start = clock();
    std::list<int> result = Viterbi(graph, 1, sigma);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: ";
    for (int vertex : result)
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    std::list<int> solution = {};
    REQUIRE(result == solution);
}

TEST_CASE("(a) 4")
{
    std::cout << "------------------------" << std::endl;
    Graph graph = {
        { {}  },
        { { { 2, 10, 0 }, { 3, 10, 0 } } },
        { { { 4, 20, 0 } } },
        { { { 4, 20, 0 } } },
        { { { 5, 30, 0 } } },
        { { { 3, 40, 0 } } }
    };
    std::vector<int> sigma = { 10, 20, 30, 40, 20, 30 };
    clock_t start = clock();
    std::list<int> result = Viterbi(graph, 1, sigma);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: ";
    for (int vertex : result)
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    std::list<int> solution = { 1, 2, 4, 5, 3, 4, 5 };
    REQUIRE(result == solution);
}

TEST_CASE("(b) 1")
{
    std::cout << "------------------------" << std::endl;
    Graph graph = {
        { {} },
        { { { 2, 30, 0.1 }, { 3, 40, 0.1 }, { 5, 30, 0.8 } } },
        { { { 6, 10, 0.6 }, { 7, 10, 0.47 } } },
        { { { 2, 10, 0.5 }, { 4, 20, 0.5 } } },
        { { { 2, 40, 0.6 }, { 5, 30, 0.4 } } },
        { { { 3, 40, 0.5 } } },
        { { { 4, 20, 1 } } },
        { { { 1, 20, 0.9 }, { 6, 30, 0.1 } } },
    };
    std::vector<int> sigma = { 10, 20, 30, 40 };
    clock_t start = clock();
    std::list<int> result = ViterbiProbability(graph, 2, sigma);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: ";
    for (int vertex : result)
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    std::list<int> solution = { 2, 7, 1, 5, 3 };
    REQUIRE(result == solution);
}

TEST_CASE("(b) 2")
{
    std::cout << "------------------------" << std::endl;
    Graph graph = {
        { {} },
        { { { 2, 30, 0.3 }, { 3, 40, 0.3 }, { 5, 30, 0.6 } } },
        { { { 6, 10, 0.6 }, { 7, 10, 0.47 } } },
        { { { 2, 10, 0.5 }, { 4, 20, 0.5 } } },
        { { { 2, 40, 0.45 }, { 5, 30, 0.55 } } },
        { { { 3, 40, 0.5 } } },
        { { { 4, 20, 1 } } },
        { { { 1, 20, 0.9 }, { 6, 30, 0.1 } } },
    };
    std::vector<int> sigma = { 10, 20, 30, 40 };
    clock_t start = clock();
    std::list<int> result = ViterbiProbability(graph, 2, sigma);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: ";
    for (int vertex : result)
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    std::list<int> solution = { 2, 6, 4, 5, 3 };
    REQUIRE(result == solution);
}

TEST_CASE("(b) 3")
{
    std::cout << "------------------------" << std::endl;
    Graph graph = {
        { {} },
        { { { 2, 10, 0.5 }, { 3, 10, 0.5 } } },
        { { { 4, 20, 1 } } },
        { { { 4, 20, 1 } } },
        { { { 5, 30, 1 } } },
        { { { 3, 40, 1 } } }
    };
    std::vector<int> sigma = { 10, 20, 30, 40, 20, 50 };
    clock_t start = clock();
    std::list<int> result = ViterbiProbability(graph, 1, sigma);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: ";
    for (int vertex : result)
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    std::list<int> solution = {};
    REQUIRE(result == solution);
}
