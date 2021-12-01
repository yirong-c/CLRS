#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>

/**
 * running time: O(n)
 * n: n-th fibonacci number (must greater than 0)
 */
int FibonacciNumber(int n)
{
    int *f, i, result;
    f = new int[n + 1];
    f[0] = 0;
    f[1] = 1;
    for (i = 2; i <= n; ++i)
        f[i] = f[i - 1] + f[i - 2];
    result = f[n];
    delete[] f;
    return result;
}

TEST_CASE()
{
    REQUIRE(1 == FibonacciNumber(1));
    REQUIRE(1 == FibonacciNumber(2));
    REQUIRE(2 == FibonacciNumber(3));
    REQUIRE(3 == FibonacciNumber(4));
    REQUIRE(5 == FibonacciNumber(5));
    REQUIRE(8 == FibonacciNumber(6));
    REQUIRE(13 == FibonacciNumber(7));
    REQUIRE(21 == FibonacciNumber(8));
    REQUIRE(34 == FibonacciNumber(9));
    REQUIRE(55 == FibonacciNumber(10));
}
