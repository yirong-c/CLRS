#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <vector>

#include "augment_red_black_tree/order_statistics_tree/order_statistics_tree.hpp"

template <typename Key>
size_t CountInversions(std::vector<Key> array)
{
    size_t inversions, i, rank;
    OrderStatisticsTree<Key, int> tree;
    std::pair<typename OrderStatisticsTree<Key, int>::Iterator, bool> insert_result;
    inversions = 0;
    for (i = 0; i < array.size(); ++i)
    {
        insert_result = tree.Insert({array[i], 0});
        rank = tree.Rank(insert_result.first);
        inversions += (1 + i - rank); 
    }
    return inversions;
}

TEST_CASE()
{
    std::vector<int> array;
    array = { 2,3,8,6,1 };
    REQUIRE(CountInversions(array) == 5);
    array = { 1,2,3,4,5 };
    REQUIRE(CountInversions(array) == 0);
    array = { 5,4,3,2,1 };
    REQUIRE(CountInversions(array) == 10);
}
