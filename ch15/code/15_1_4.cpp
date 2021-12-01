#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>

#include <limits.h>
#include <vector>
#include <list>

/**
 * p: table of prices (index start from 0)
 * n: length of rod
 * r: table of maximum revenue (index start from 1)
 * s: table of optimal size i of the first piece to cut off (index start from 1)
 * return maximum revenue
 */
int ExtendedMemoizedCutRodAux(const std::vector<int>& p, int n, int *r, int *s)
{
    int q, i, reminder_r;
    if (r[n] >= 0) return r[n];
    q = INT_MIN;
    for (i = 0; i < n; ++i)
    {
        reminder_r = ExtendedMemoizedCutRodAux(p, n - i - 1, r, s);
        if (q < p[i] + reminder_r)
        {
            q = p[i] + reminder_r;
            s[n] = i + 1;
        }
    }
    r[n] = q;
    return q;
}

/**
 * running time: O(n^2)
 * p: table of prices (index start from 0)
 * n: length of rod
 * return (r, s)
 * r: table of maximum revenue (index start from 1)
 * s: table of optimal size i of the first piece to cut off (index start from 1)
 * caller is responsible to deallocate return value r and s
 */
std::pair<int*, int*> ExtendedMemoizedCutRod(const std::vector<int>& p, int n)
{
    int *r, *s, i;
    r = new int[n + 1];
    s = new int[n + 1];
    r[0] = 0;
    s[0] = 0;
    for (i = 1; i <= n; ++i) r[i] = INT_MIN;
    ExtendedMemoizedCutRodAux(p, n, r, s);
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
    result = ExtendedMemoizedCutRod(p, n);
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

TEST_CASE("ExtendedMemoizedCutRod")
{
    std::pair<int*, int*> result;
    std::vector<int> p;
    int solution_r[] = { 0, 1, 5, 8, 10, 13, 17, 18, 22, 25, 30 };
    int solution_s[] = { 0, 1, 2, 3, 2, 2, 6, 1, 2, 3, 10 };
    p = {1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    result = ExtendedMemoizedCutRod(p, 10);
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
