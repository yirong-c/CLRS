#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>

#include <vector>

#include "augment_red_black_tree/order_statistics_tree/order_statistics_tree.hpp"

/**
 * running time: O(nlgn)
 * caller is responsible to deallocate the return value
 * n, m must greater than 0
 */
std::vector<int>* GetJosephusPermutation(int n, int m)
{
    int i, rank;
    OrderStatisticsTree<int, int> ostree;
    OrderStatisticsTree<int, int>::Iterator it;
    std::vector<int>* result;
    /* build order statistics tree: O(nlgn) */
    for (i = 1; i <= n; ++i) ostree.Insert(std::make_pair(i, 0));
    /* allocate vector */
    result = new std::vector<int>;
    result->reserve(n);
    /* output: O(nlgn) */
    rank = m;
    for (i = n; i > 0; --i)// n times
    {
        rank = ((rank - 1) % i) + 1;
        it = ostree.Select(rank);// O(lgn)
        result->push_back(it->first);
        ostree.Delete(it);// O(lgn)
        rank += (m - 1);
    }
    return result;
}

TEST_CASE("example")
{
    std::vector<int>* result, solution;
    result = GetJosephusPermutation(7, 3);
    solution = {3, 6, 2, 7, 5, 1, 4};
    REQUIRE(*result == solution);
    delete result;
}

TEST_CASE("edge case 1")
{
    std::vector<int>* result, solution;
    result = GetJosephusPermutation(1, 3);
    solution = { 1 };
    REQUIRE(*result == solution);
    delete result;
}

TEST_CASE("edge case 2")
{
    std::vector<int>* result, solution;
    result = GetJosephusPermutation(3, 1);
    solution = { 1, 2, 3 };
    REQUIRE(*result == solution);
    delete result;
}
