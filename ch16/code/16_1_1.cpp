#include <vector>
#include <list>

struct Activity
{
    int s;
    int f;
};

void OutputAux(const std::vector< std::vector<int> >& dp_selection, 
    int i, int j, std::list<int>& output)
{
    if (dp_selection[i][j] > 0)
    {
        OutputAux(dp_selection, i, dp_selection[i][j], output);
        output.push_back(dp_selection[i][j] - 1);
        OutputAux(dp_selection, dp_selection[i][j], j, output);
    }
}

// assume intervals are sorted by finish time
std::list<int> DpActivitySelector(const std::vector<Activity>& intervals)
{
    int n, i, j, l, k, l_size;
    n = (int)(intervals.size());
    // dp_size index start by 1
    std::vector< std::vector<int> > dp_size(n + 2, std::vector<int>(n + 2, 0)), 
        dp_selection(n + 2, std::vector<int>(n + 2, -1));
    // compute
    for (l = 2; l <= n + 1; ++l)
    {
        for (i = 0; i <= n + 1 - l; ++i)
        {
            j = i + l;
            for (k = i + 1; k <= j - 1; ++k)
            {
                if ((i == 0 || intervals[k - 1].s >= intervals[i - 1].f) && 
                    (j == n + 1 || intervals[k - 1].f <= intervals[j - 1].s))
                {
                    l_size = dp_size[i][k] + dp_size[k][j] + 1;
                    if (dp_size[i][j] < l_size)
                    {
                        dp_size[i][j] = l_size;
                        dp_selection[i][j] = k;
                    }
                }
            }
        }
    }
    // output
    std::list<int> output;
    OutputAux(dp_selection, 0, n + 1, output);
    return output;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("Example")
{
    std::cout << "------------------------" << std::endl;
    std::vector<Activity> intervals = { { 1, 4 }, { 3, 5 }, { 0, 6 }, { 5, 7 }, { 3, 9 },
        { 5, 9 }, { 6, 10 }, { 8, 11 }, { 8, 12 }, { 2, 14 }, { 12, 16 } };
    clock_t start = clock();
    std::list<int> output = DpActivitySelector(intervals);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::list<int> solution = { 0, 3, 7, 10 };
    REQUIRE(output == solution);
}
