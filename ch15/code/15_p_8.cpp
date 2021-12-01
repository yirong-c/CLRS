#include <vector>
#include <algorithm>

std::vector<int> LowestDisruptionSeam(const std::vector< std::vector<int> >& disruption)
{
    int m, n, i, j;
    m = disruption.size();
    n = disruption[0].size();
    std::vector< std::vector<int> > dp_disruption_sum(m, std::vector<int>(n)), 
        dp_col_of_next_row(m - 1, std::vector<int>(n));
    std::vector<int> result;
    result.reserve(m);
    // compute
    for (j = 0; j < n; ++j)
    {
        dp_disruption_sum[m - 1][j] = disruption[m - 1][n];
    }
    for (i = m - 2; i >= 0; --i)
    {
        if (dp_disruption_sum[i + 1][0] < dp_disruption_sum[i + 1][1])
        {
            dp_disruption_sum[i][0] = dp_disruption_sum[i + 1][0] + disruption[i][0];
            dp_col_of_next_row[i][0] = 0;
        }
        else
        {
            dp_disruption_sum[i][0] = dp_disruption_sum[i + 1][1] + disruption[i][0];
            dp_col_of_next_row[i][0] = 1;
        }
        for (j = 1; j < n - 1; ++j)
        {
            if (dp_disruption_sum[i + 1][j - 1] < dp_disruption_sum[i + 1][j])
            {
                dp_disruption_sum[i][j] = dp_disruption_sum[i + 1][j - 1];
                dp_col_of_next_row[i][j] = j - 1;
            }
            else
            {
                dp_disruption_sum[i][j] = dp_disruption_sum[i + 1][j];
                dp_col_of_next_row[i][j] = j;
            }
            if (dp_disruption_sum[i + 1][j + 1] < dp_disruption_sum[i][j])
            {
                dp_disruption_sum[i][j] = dp_disruption_sum[i + 1][j + 1];
                dp_col_of_next_row[i][j] = j + 1;
            }
            dp_disruption_sum[i][j] += disruption[i][j];
        }
        if (dp_disruption_sum[i + 1][n - 2] < dp_disruption_sum[i + 1][n - 1])
        {
            dp_disruption_sum[i][n - 1] = dp_disruption_sum[i + 1][n - 2] + 
                disruption[i][n - 1];
            dp_col_of_next_row[i][n - 1] = n - 2;
        }
        else
        {
            dp_disruption_sum[i][n - 1] = dp_disruption_sum[i + 1][n - 1] + 
                disruption[i][n - 1];
            dp_col_of_next_row[i][n - 1] = n - 1;
        }
    }
    // output
    j = std::max_element(dp_disruption_sum.begin(), dp_disruption_sum.end()) - 
        dp_disruption_sum.begin();
    result.push_back(j);
    for (i = 0; i < m - 1; ++i)
    {
        j = dp_col_of_next_row[0][j];
        result.push_back(j);
    }
    return result;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE()
{
    std::vector< std::vector<int> > disruption = {
        { 5, 4, 25 },
        { 29, 10, 0 },
        { 1, 9, 5 }
    };
    clock_t start = clock();
    std::vector<int> result = LowestDisruptionSeam(disruption);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: ";
    for (int col : result)
    {
        std::cout << col << " ";
    }
    std::cout << std::endl;
    std::vector<int> solution = { 1, 2, 2 };
    REQUIRE(result == solution);
}
