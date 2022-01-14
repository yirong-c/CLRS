#include <list>
#include <vector>

#include "selection.hpp"

struct Item
{
    int w;// weight
    int v;// value
    double v_div_w;// value per pound
};

// return list of pair where the first element of the pair is 
// the index of item in the container after the funtion returns 
// and the second element of the pair is the weight of the item in the knapsack
std::list< std::pair<int, int> > FractionalKnapsack
    (std::vector<Item>& items, int maximum_weight)
{
    std::list< std::pair<int, int> > output;
    int weight_sum = 0;
    for (Item& item : items)
    {
        weight_sum += item.w;
    }
    if (weight_sum <= maximum_weight)
    {
        // put all items into kanpsack
        for (size_t i = 0; i < items.size(); ++i)
        {
            output.emplace_back(i, items[i].w);
        }
    }
    else
    {
        for (Item& item : items)
        {
            item.v_div_w = (double)(item.v) / (item.w);
        }
        // note that a partition around the pivot will be performed also
        cotl::WeightedSelect(items.begin(), items.end(), maximum_weight - 1, 
            [](const Item& a) { return a.w; }, 
            [](const Item& a, const Item& b) { return b.v_div_w - a.v_div_w; });
        for (size_t i = 0; i < items.size(); ++i)
        {
            int pick_weight = std::min(maximum_weight, items[i].w);
            maximum_weight -= pick_weight;
            output.emplace_back(i, pick_weight);
            if (maximum_weight == 0)
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

void CheckItems(const std::vector<Item>& a, const std::vector<Item>& b)
{
    assert(a.size() == b.size());
    for (size_t i = 0; i < a.size(); ++i)
    {
        assert(a[i].w == b[i].w);
        assert(a[i].v == b[i].v);
        assert(a[i].v_div_w == b[i].v_div_w);
    }
}

TEST_CASE("1")
{
    std::cout << "------------------------" << std::endl;
    std::vector<Item> items = 
        { { 20, 100, 0 }, { 10, 60, 0 }, { 30, 120, 0 } };
    clock_t start = clock();
    std::list< std::pair<int, int> > output = FractionalKnapsack(items, 50);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::vector<Item> items_solution = { { 10, 60, 6 }, { 20, 100, 5 }, { 30, 120, 4 } };
    std::list< std::pair<int, int> > output_solution = { { 0, 10 }, { 1, 20 }, { 2, 20 } };
    CheckItems(items, items_solution);
    REQUIRE(output == output_solution);
}
