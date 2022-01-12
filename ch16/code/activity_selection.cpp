#include <vector>
#include <list>

struct Activity
{
    int s;
    int f;
};

// assume intervals are sorted by finish time
std::list<int> RecursiveActivitySelector(const std::vector<Activity>& intervals, int k)
{
    int m;
    m = k + 1;
    while (m < intervals.size() && intervals[m].s < intervals[k].f)
    {
        ++m;
    }
    if (m < intervals.size())
    {
        std::list<int> activities = RecursiveActivitySelector(intervals, m);
        activities.push_front(m);
        return activities;
    }
    else
    {
        return std::list<int>();
    }
}

// assume f is sorted
std::list<int> GreedyActivitySelector(const std::vector<Activity>& intervals)
{
    int k, m, n;
    std::list<int> activities;
    n = (int)(intervals.size());
    activities.push_back(0);
    k = 0;
    for (m = 1; m < n; ++m)
    {
        if (intervals[m].s >= intervals[k].f)
        {
            activities.push_back(m);
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

TEST_CASE("Example RecursiveActivitySelector")
{
    std::cout << "------------------------" << std::endl;
    std::vector<Activity> intervals = { { 1, 4 }, { 3, 5 }, { 0, 6 }, { 5, 7 }, { 3, 9 },
        { 5, 9 }, { 6, 10 }, { 8, 11 }, { 8, 12 }, { 2, 14 }, { 12, 16 } };
    clock_t start = clock();
    std::list<int> output = RecursiveActivitySelector(intervals, -1);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::list<int> solution = { 0, 3, 7, 10 };
    REQUIRE(output == solution);
}

TEST_CASE("Example GreedyActivitySelector")
{
    std::cout << "------------------------" << std::endl;
    std::vector<Activity> intervals = { { 1, 4 }, { 3, 5 }, { 0, 6 }, { 5, 7 }, { 3, 9 },
        { 5, 9 }, { 6, 10 }, { 8, 11 }, { 8, 12 }, { 2, 14 }, { 12, 16 } };
    clock_t start = clock();
    std::list<int> output = GreedyActivitySelector(intervals);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::list<int> solution = { 0, 3, 7, 10 };
    REQUIRE(output == solution);
}
