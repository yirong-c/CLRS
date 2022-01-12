#include <vector>
#include <list>

struct Activity
{
    int s;
    int f;
};

// assume intervals are sorted by start time
std::list<int> GreedyActivitySelector(const std::vector<Activity>& intervals)
{
    int k, m, n;
    std::list<int> activities;
    n = (int)(intervals.size());
    activities.push_front(n - 1);
    k = n - 1;
    for (m = n - 2; m >= 0; --m)
    {
        if (intervals[k].s >= intervals[m].f)
        {
            activities.push_front(m);
            k = m;
        }
    }
    return activities;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("Example")
{
    std::cout << "------------------------" << std::endl;
    std::vector<Activity> intervals = { { 0, 6 }, { 1, 4 }, { 2, 14 }, { 3, 5 }, { 3, 9 }, 
        { 5, 7 }, { 5, 9 }, { 6, 10 }, { 8, 11 }, { 8, 12 }, { 12, 16 } };
    clock_t start = clock();
    std::list<int> output = GreedyActivitySelector(intervals);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::list<int> solution = { 3, 5, 9, 10 };
    REQUIRE(output == solution);
}
