#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>

#include <limits.h>
#include <vector>
#include <list>

/*****************************************************
 * recursive top-down implementation
 *****************************************************/

/**
 * running time: O(2^n)
 * p: table of prices (index start from 0)
 * n: length of rod
 * return maximum revenue
 */
int CutRod(const std::vector<int>& p, int n)
{
    int q, i;
    if (n == 0) return 0;
    q = INT_MIN;
    for (i = 0; i < n; ++i) 
        q = std::max(q, p[i] + CutRod(p, n - i - 1));
    return q;
}

/*****************************************************
 * top-down implementation (dynamic programming)
 *****************************************************/

/**
 * p: table of prices (index start from 0)
 * n: length of rod
 * r: table of maximum revenue (index start from 1)
 * return maximum revenue
 */
int MemoizedCutRodAux(const std::vector<int>& p, int n, int *r)
{
    int q, i;
    if (r[n] >= 0) return r[n];
    if (n == 0)
    {
        q = 0;
    }
    else
    {
        q = INT_MIN;
        for (i = 0; i < n; ++i)
            q = std::max(q, p[i] + MemoizedCutRodAux(p, n - i - 1, r));
    }
    r[n] = q;
    return q;
}

/**
 * running time: O(n^2)
 * p: table of prices (index start from 0)
 * n: length of rod
 * return maximum revenue
 */
int MemoizedCutRod(const std::vector<int>& p, int n)
{
    int *r, q, i;
    r = new int[n + 1];
    for (i = 0; i <= n; ++i) r[i] = INT_MIN;
    q = MemoizedCutRodAux(p, n, r);
    delete[] r;
    return q;
}

/*****************************************************
 * bottom-up implementation (dynamic programming)
 *****************************************************/

/**
 * running time: O(n^2)
 * p: table of prices (index start from 0)
 * n: length of rod
 * return maximum revenue
 */
int BottomUpCutRod(const std::vector<int>& p, int n)
{
    int *r, q, i, j;
    r = new int[n + 1];
    r[0] = 0;
    for (j = 1; j <= n; ++j)
    {
        q = INT_MIN;
        for (i = 0; i < j; ++i)
            q = std::max(q, p[i] + r[j - i - 1]);
        r[j] = q;
    }
    delete[] r;
    return q;
}

/*****************************************************
 * extended bottom-up implementation 
 * (dynamic programming) (output solution)
 *****************************************************/

/**
 * running time: O(n^2)
 * p: table of prices (index start from 0)
 * n: length of rod
 * return (r, s)
 * r: table of maximum revenue (index start from 1)
 * s: table of optimal size i of the first piece to cut off (index start from 1)
 * caller is responsible to deallocate return value r and s
 */
std::pair<int*, int*> ExtendedBottomUpCutRod(const std::vector<int>& p, int n)
{
    int *r, *s, q, i, j;
    r = new int[n + 1];
    s = new int[n + 1];
    r[0] = 0;
    s[0] = 0;
    for (j = 1; j <= n; ++j)
    {
        q = INT_MIN;
        for (i = 0; i < j; ++i)
        {
            if (q < p[i] + r[j - i - 1])
            {
                q = p[i] + r[j - i - 1];
                s[j] = i + 1;
            }
        }
        r[j] = q;
    }
    return std::make_pair(r, s);
}

/**
 * p: table of prices (index start from 0)
 * n: length of rod
 * return solution
 * caller is responsible to deallocate return value
 */
std::list<int>* PrintCutRodSolution(const std::vector<int>& p, int n)
{
    std::pair<int*, int*> result;
    std::list<int>* solution;
    result = ExtendedBottomUpCutRod(p, n);
    solution = new std::list<int>;
    while (n > 0)
    {
        solution->push_back(result.second[n]);
        n -= result.second[n];
    }
    delete[] result.first;
    delete[] result.second;
    return solution;
}

/*****************************************************
 * test cases
 *****************************************************/

typedef int (*TestFunc)(const std::vector<int>& p, int n);

TEST_CASE("CutRod")
{
    std::vector<int> p;
    TestFunc test_func;
    test_func = CutRod;
    p = {1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    REQUIRE(test_func(p, 1) == 1);
    REQUIRE(test_func(p, 2) == 5);
    REQUIRE(test_func(p, 3) == 8);
    REQUIRE(test_func(p, 4) == 10);
    REQUIRE(test_func(p, 5) == 13);
    REQUIRE(test_func(p, 6) == 17);
    REQUIRE(test_func(p, 7) == 18);
    REQUIRE(test_func(p, 8) == 22);
    REQUIRE(test_func(p, 9) == 25);
    REQUIRE(test_func(p, 10) == 30);
}

TEST_CASE("MemoizedCutRod")
{
    std::vector<int> p;
    TestFunc test_func;
    test_func = MemoizedCutRod;
    p = {1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    REQUIRE(test_func(p, 1) == 1);
    REQUIRE(test_func(p, 2) == 5);
    REQUIRE(test_func(p, 3) == 8);
    REQUIRE(test_func(p, 4) == 10);
    REQUIRE(test_func(p, 5) == 13);
    REQUIRE(test_func(p, 6) == 17);
    REQUIRE(test_func(p, 7) == 18);
    REQUIRE(test_func(p, 8) == 22);
    REQUIRE(test_func(p, 9) == 25);
    REQUIRE(test_func(p, 10) == 30);
}

TEST_CASE("BottomUpCutRod")
{
    std::vector<int> p;
    TestFunc test_func;
    test_func = BottomUpCutRod;
    p = {1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    REQUIRE(test_func(p, 1) == 1);
    REQUIRE(test_func(p, 2) == 5);
    REQUIRE(test_func(p, 3) == 8);
    REQUIRE(test_func(p, 4) == 10);
    REQUIRE(test_func(p, 5) == 13);
    REQUIRE(test_func(p, 6) == 17);
    REQUIRE(test_func(p, 7) == 18);
    REQUIRE(test_func(p, 8) == 22);
    REQUIRE(test_func(p, 9) == 25);
    REQUIRE(test_func(p, 10) == 30);
}

TEST_CASE("ExtendedBottomUpCutRod")
{
    std::pair<int*, int*> result;
    std::vector<int> p;
    int solution_r[] = { 0, 1, 5, 8, 10, 13, 17, 18, 22, 25, 30 };
    int solution_s[] = { 0, 1, 2, 3, 2, 2, 6, 1, 2, 3, 10 };
    p = {1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    result = ExtendedBottomUpCutRod(p, 10);
    REQUIRE(memcmp(solution_r, result.first, sizeof(solution_r)) == 0);
    REQUIRE(memcmp(solution_s, result.second, sizeof(solution_s)) == 0);
    delete[] result.first;
    delete[] result.second;
}

TEST_CASE("PrintCutRodSolution")
{
    std::list<int> *solution_result, solution_solution;
    std::vector<int> p;
    p = {1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    SECTION("n = 10")
    {
        solution_solution = { 10 };
        solution_result = PrintCutRodSolution(p, 10);
        REQUIRE(solution_solution == *solution_result);
        delete solution_result;
    }
    SECTION("n = 10")
    {
        solution_solution = { 1, 6 };
        solution_result = PrintCutRodSolution(p, 7);
        REQUIRE(solution_solution == *solution_result);
        delete solution_result;
    }
}
