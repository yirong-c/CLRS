#include <list>
#include <vector>

struct Item
{
    int w;// weight
    int v;// value
};

// assume v mono decreasing and w mono increasing
std::list<int> ZeroOneKnapsack(const std::vector<Item>& items, int maximum_weight)
{
    int k, n;
    std::list<int> output;
    n = (int)(items.size());
    for (k = 0; k < n; ++k)
    {
        if (items[k].w <= maximum_weight)
        {
            maximum_weight -= items[k].w;
            output.push_back(k);
        }
        else
        {
            break;
        }
    }
    return output;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("1")
{
    std::cout << "------------------------" << std::endl;
    std::vector<Item> items = { { 10, 120 }, { 20, 100 }, { 30, 80 } };
    clock_t start = clock();
    std::list<int> output = ZeroOneKnapsack(items, 50);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::list<int> solution = { 0, 1 };
    REQUIRE(output == solution);
}
