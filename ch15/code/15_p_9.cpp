#include <limits.h>
#include <vector>

int Partition(std::vector<int>& arr, int left, int right)
{
    int i, j, pivot_val;
    i = left - 1;
    pivot_val = arr[right];
    for (j = left; j < right; ++j)
    {
        if (arr[j] <= pivot_val)
        {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    ++i;
    std::swap(arr[i], arr[right]);
    return i;
}

void Quicksort(std::vector<int>& arr, int left, int right)
{
    int pivot_index;
    if (left < right)
    {
        pivot_index = Partition(arr, left, right);
        Quicksort(arr, left, pivot_index - 1);
        Quicksort(arr, pivot_index + 1, right);
    }
}

void OutputSequence(const std::vector<int>& breaking_point, 
    const std::vector< std::vector<int> >& dp_break,
    int i, int j, std::vector<int>& result)
{
    int x;
    if (j - i == 1)
        return;
    x = dp_break[i][j];
    result.push_back(breaking_point[x]);
    OutputSequence(breaking_point, dp_break, i, x, result);
    OutputSequence(breaking_point, dp_break, x, j, result);
}

std::pair<int, std::vector<int> > BreakingString(std::vector<int>& breaking_point, int n)
{
    int m, l, i, j, x, l_cost;
    m = (int)(breaking_point.size());
    std::vector< std::vector<int> > dp_cost(m + 2, std::vector<int>(m + 2)),
        dp_break(m + 2, std::vector<int>(m + 2));
    std::vector<int> result;
    result.reserve(m);
    breaking_point.push_back(breaking_point[0]);
    breaking_point[0] = 0;
    breaking_point.push_back(n);
    Quicksort(breaking_point, 1, m);
    for (i = 0; i <= m; ++i)// l = 1
    {
        dp_cost[i][i + 1] = 0;
    }
    for (l = 2; l <= m + 2; ++l)
    {
        for (i = 0; i <= m + 1 - l; ++i)
        {
            j = i + l;
            dp_cost[i][j] = INT_MAX;
            for (x = i + 1; x < j; ++x)
            {
                l_cost = dp_cost[i][x] + dp_cost[x][j];
                if (l_cost < dp_cost[i][j])
                {
                    dp_cost[i][j] = l_cost;
                    dp_break[i][j] = x;
                }
            }
            dp_cost[i][j] += breaking_point[j] - breaking_point[i];
        }
    }
    // output
    OutputSequence(breaking_point, dp_break, 0, m + 1, result);
    return std::make_pair(dp_cost[0][m + 1], result);
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE()
{
    std::vector<int> breaking_point = { 2, 10, 8 };
    clock_t start = clock();
    std::pair<int, std::vector<int> > result = BreakingString(breaking_point, 20);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: \n";
    std::cout << "cost: " << result.first << "\n";
    std::cout << "sequence: ";
    for (int point : result.second)
    {
        std::cout << point << " ";
    }
    std::cout << std::endl;
    std::vector<int> solution = { 10, 2, 8 };
    REQUIRE(result.first == 38);
    REQUIRE(result.second == solution);
}
