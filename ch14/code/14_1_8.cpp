#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <utility>
#include <vector>
#include <cmath>

#include "augment_red_black_tree/order_statistics_tree/order_statistics_tree.hpp"

// center of cycle is (0,0)

struct Endpoint
{
    double x;
    double y;

    bool operator<(const Endpoint& other) const
    {
        if (this->x >= 0 && other.x >= 0)
            return this->y > other.y;
        else if (this->x < 0 && other.x < 0)
            return this->y < other.y;
        else
            return this->x >= 0 && other.x < 0;
    }

    bool operator==(const Endpoint& other) const
    {
        return this->x == other.x && this->y == other.y;
    }
};

struct Chord
{
    Endpoint endpoint_a;
    Endpoint endpoint_b;
};

typedef OrderStatisticsTree<Endpoint, int> OSTree;

size_t CountIntersections(std::vector<Chord> chords)
{
    size_t intersections, i, rank_a, rank_b, rank_diff_1, rank_diff_2;
    OSTree tree;
    std::pair<typename OSTree::Iterator, bool> insert_result_a, insert_result_b;
    intersections = 0;
    for (i = 0; i < chords.size(); ++i)
    {
        insert_result_a = tree.Insert({chords[i].endpoint_a, 0});
        insert_result_b = tree.Insert({chords[i].endpoint_b, 0});
        rank_a = tree.Rank(insert_result_a.first);
        rank_b = tree.Rank(insert_result_b.first);
        if (rank_a > rank_b) std::swap (rank_a, rank_b);
        // rank_a must smaller than rank_b
        rank_diff_1 = rank_b - rank_a - 1;
        rank_diff_2 = tree.Size() - rank_b + rank_a - 1;
        intersections += std::min(rank_diff_1, rank_diff_2); 
    }
    return intersections;
}

TEST_CASE()
{
    // cycle: x^{2}+y^{2}=100
    std::vector<Chord> chords;
    chords = {
        { { -8.41, -5.41 }, { 5.41, 8.41 } },// y=x+3
        { { 4.085, 9.128 }, { -6.854, -7.281 } },// y=1.5x+3
        { { 3.403, 9.403 }, { -9.403, -3.403 } },// y=x+6
        { { 0.2, -9.998 }, { -3.661, 9.306 } },// y=-5x-9
        { { -4.472, 8.944 }, { 4.472, -8.944 } }// y=-2x
    };
    REQUIRE(CountIntersections(chords) == 8);
}
