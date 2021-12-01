#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>

#include <limits.h>
#include <vector>

/**
 * running time: O(n^2)
 * p: table of prices (index start from 0)
 * n: length of rod
 * c: cost of each cut
 * return maximum revenue
 */
int BottomUpCutRodWithCost(const std::vector<int>& p, int n, int c)
{
    int *r, q, i, j;
    r = new int[n + 1];
    r[0] = 0;
    for (j = 1; j <= n; ++j)
    {
        q = p[j - 1];
        for (i = 0; i < j - 1; ++i)
            q = std::max(q, p[i] + r[j - i - 1] - c);
        r[j] = q;
    }
    delete[] r;
    return q;
}

TEST_CASE("BottomUpCutRodWithCost")
{
    std::vector<int> p;
    p = {1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    REQUIRE(BottomUpCutRodWithCost(p, 1, 2) == 1);
    REQUIRE(BottomUpCutRodWithCost(p, 2, 2) == 5);
    REQUIRE(BottomUpCutRodWithCost(p, 3, 2) == 8);
    REQUIRE(BottomUpCutRodWithCost(p, 4, 2) == 9);
    REQUIRE(BottomUpCutRodWithCost(p, 5, 2) == 11);
}
