#include <vector>
#include <algorithm>

std::pair<double, std::vector<int> > InvestmentStrategy
    (const std::vector< std::vector<double> >& rate, int f_1, int f_2)
{
    int i, j, k, n;
    double l_money;
    n = (int)(rate.size());
    std::vector< std::vector<double> > dp_money(n, std::vector<double>(10)); 
    std::vector< std::vector<int> > dp_investment(n, std::vector<int>(10));
    std::vector<int> result(10);
    // compute
    for (i = 0; i < n; ++i)// j == 0
    {
        dp_money[i][0] = rate[i][0] * 10000;
    }
    for (j = 1; j < 10; ++j)
    {
        for (i = 0; i < n; ++i)
        {
            dp_money[i][j] = (dp_money[i][j - 1] - f_1) * rate[i][j];
            dp_investment[i][j] = i;
            for (k = 0; k < n; ++k)
            {
                l_money = (dp_money[k][j - 1] - f_2) * rate[i][j];
                if (l_money > dp_money[i][j])
                {
                    dp_money[i][j] = l_money;
                    dp_investment[i][j] = k;
                }
            }
        }
    }
    // output
    l_money = dp_money[0][9];
    result[9] = 0;
    for (i = 1; i < n; ++i)
    {
        if (dp_money[i][9] > l_money)
        {
            l_money = dp_money[i][9];
            result[9] = i;
        }
    }
    for (j = 9; j > 0; --j)
    {
        result[j - 1] = dp_investment[result[j]][j];
    }
    return std::make_pair(l_money, std::move(result));
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE()
{
    std::vector< std::vector<double> > rate = { 
        { 1.1, 1.2, 1.3, 1.4, 1.5, 1.4, 1.2, 1.6, 1.7, 1.4 }, 
        { 1.1, 1.5, 1.6, 1.3, 1.5, 1.5, 1.7, 1.4, 1.2, 1.5 }, 
        { 1.5, 1.2, 1.4, 1.7, 1.2, 1.3, 1.5, 1.3, 1.4, 1.1 }
    };
    clock_t start = clock();
    std::pair<double, std::vector<int> > result = 
        InvestmentStrategy(rate, 300, 500);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: \n";
    std::cout << "amount: " << result.first << "\n";
    for (int i : result.second)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    // std::vector<int> solution;
    // REQUIRE(true);
}
