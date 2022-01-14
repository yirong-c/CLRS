#include <list>
#include <vector>

struct Item
{
    int w;// weight
    int v;// value
};

std::list<int> ZeroOneKnapsack(const std::vector<Item>& items, int maximum_weight)
{
    int n, i, j;
    n = (int)(items.size());
    std::vector< std::vector<int> > dp_value(n, std::vector<int>(maximum_weight + 1));
    std::vector< std::vector<bool> > dp_put(n, std::vector<bool>(maximum_weight + 1));
    std::list<int> output;
    // compute
    i = n - 1;
    for (j = 0; j <= maximum_weight; ++j)
    {
        if (j >= items[i].w)
        {
            dp_value[i][j] = items[i].v;
            dp_put[i][j] = true;
        }
        else
        {
            dp_value[i][j] = 0;
            dp_put[i][j] = false;
        }
    }
    for (i = n - 2; i >= 0; --i)
    {
        for (j = 0; j <= maximum_weight; ++j)
        {
            if (j >= items[i].w && dp_value[i + 1][j - items[i].w] + items[i].v > dp_value[i + 1][j])
            {
                dp_value[i][j] = dp_value[i + 1][j - items[i].w] + items[i].v;
                dp_put[i][j] = true;
            }
            else
            {
                dp_value[i][j] = dp_value[i + 1][j];
                dp_put[i][j] = false;
            }
        }
    }
    // output
    j = maximum_weight;
    for (i = 0; i < n; ++i)
    {
        if (dp_put[i][j])
        {
            output.push_back(i);
            j -= items[i].w;
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
    std::vector<Item> items = { { 10, 60 }, { 20, 100 }, { 30, 120 } };
    clock_t start = clock();
    std::list<int> output = ZeroOneKnapsack(items, 50);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::list<int> solution = { 1, 2 };
    REQUIRE(output == solution);
}
