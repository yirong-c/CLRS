#include <limits.h>
#include <vector>

std::pair<int, std::vector<int> > InventoryPlanning
    (const std::vector<int>& demands, int m, int c, 
    const std::vector<int>& holding_cost)
{
    int j, k, l_cost;
    size_t n, i;
    n = demands.size();
    std::vector<int> sum_demands(n), result(n);
    // compute
    sum_demands[0] = demands[0];
    for (i = 1; i < n; ++i)
    {
        sum_demands[i] += sum_demands[i - 1] + demands[i];
    }
    std::vector< std::vector<int> > dp_cost(n, std::vector<int>(sum_demands[n - 1])),
        dp_plan(n, std::vector<int>(sum_demands[n - 1]));
    for (j = sum_demands[0]; j <= sum_demands[n - 1]; ++j)
    {
        dp_cost[0][j] = holding_cost[j - sum_demands[0]] + 
            c * std::max(j - m, 0);
    }
    for (i = 1; i < n; ++i)
    {
        for (j = sum_demands[i]; j <= sum_demands[n - 1]; ++j)
        {
            dp_cost[i][j] = INT_MAX;
            for (k = sum_demands[i - 1]; k <= j; ++k)
            {
                l_cost = dp_cost[i - 1][k] + c * std::max(j - k - m, 0);
                if (l_cost < dp_cost[i][j])
                {
                    dp_cost[i][j] = l_cost;
                    dp_plan[i][j] = k;
                }
            }
            dp_cost[i][j] += holding_cost[j - sum_demands[i]];
        }
    }
    // output
    result[n - 1] = sum_demands[n - 1];
    for (i = n - 2; i < n; --i)
    {
        result[i] = dp_plan[i + 1][result[i + 1]];
        result[i + 1] -= result[i];
    }
    return std::make_pair(dp_cost[n - 1][sum_demands[n - 1]], std::move(result));
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("1")
{
    int j;
    std::vector<int> demands = { 8, 12 };
    std::vector<int> holding_cost(21);
    holding_cost[0] = 0;
    for (j = 1; j < 21; ++j)
    {
        holding_cost[j] = holding_cost[j - 1] + 5;
    }
    clock_t start = clock();
    std::pair<int, std::vector<int> > result = 
        InventoryPlanning(demands, 10, 6, holding_cost);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: \n";
    std::cout << "cost: " << result.first << "\n";
    std::cout << "plan: ";
    for (int p : result.second)
    {
        std::cout << p << " ";
    }
    std::cout << std::endl;
    std::vector<int> solution = { 10, 10 };
    REQUIRE(result.first == 10);
    REQUIRE(result.second == solution);
}

TEST_CASE("2")
{
    int j;
    std::vector<int> demands = { 8, 12 };
    std::vector<int> holding_cost(21);
    holding_cost[0] = 0;
    for (j = 1; j < 21; ++j)
    {
        holding_cost[j] = holding_cost[j - 1] + 6;
    }
    clock_t start = clock();
    std::pair<int, std::vector<int> > result = 
        InventoryPlanning(demands, 10, 5, holding_cost);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: \n";
    std::cout << "cost: " << result.first << "\n";
    std::cout << "plan: ";
    for (int p : result.second)
    {
        std::cout << p << " ";
    }
    std::cout << std::endl;
    std::vector<int> solution = { 8, 12 };
    REQUIRE(result.first == 10);
    REQUIRE(result.second == solution);
}
