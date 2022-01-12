#include <algorithm>
#include <list>
#include <queue>
#include <utility>
#include <vector>

struct Activity
{
    int s;
    int f;
};

struct Element
{
    int interval;
    std::list< std::list<int> >::iterator list;

    Element(int interval, std::list< std::list<int> >::iterator list)
        : interval(interval), list(list) {}
};

// assume intervals are sorted by finish time
std::list< std::list<int> > IntervalGraphColoring(const std::vector<Activity>& intervals)
{
    int i, n;
    n = (int)(intervals.size());
    std::list< std::list<int> > collection;
    auto heap_cmp = [&intervals](const Element& a, const Element& b) {
        return intervals[a.interval].s < intervals[b.interval].s;
    };
    std::priority_queue<Element, std::vector<Element>, decltype(heap_cmp)> heap(heap_cmp);
    std::list< std::list<int> >::iterator curr_list;
    collection.emplace_front();
    curr_list = collection.begin();
    curr_list->push_front(n - 1);
    heap.emplace(n - 1, curr_list);
    for (i = n - 2; i >= 0; --i)
    {
        if (intervals[i].f <= intervals[heap.top().interval].s)
        {
            curr_list = heap.top().list;
            curr_list->push_front(i);
            heap.pop();
        }
        else
        {
            collection.emplace_front();
            curr_list = collection.begin();
            curr_list->push_front(i);
        }
        heap.emplace(i, curr_list);
    }
    return collection;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("1")
{
    std::cout << "------------------------" << std::endl;
    std::vector<Activity> intervals = { { 1, 4 }, { 3, 5 }, { 0, 6 }, { 5, 7 }, { 3, 9 },
        { 5, 9 }, { 6, 10 }, { 8, 11 }, { 8, 12 }, { 2, 14 }, { 12, 16 } };
    clock_t start = clock();
    std::list< std::list<int> > collection = IntervalGraphColoring(intervals);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::list< std::list<int> > solution = { { 4 }, { 5 }, { 1, 6 }, { 2, 7 }, { 9 }, { 0, 3, 8, 10 } };
    REQUIRE(collection == solution);
}

TEST_CASE("2")
{
    std::cout << "------------------------" << std::endl;
    std::vector<Activity> intervals = { { 1, 3 }, { 2, 4 }, { 4, 5 }, { 3, 6 } };
    clock_t start = clock();
    std::list< std::list<int> > collection = IntervalGraphColoring(intervals);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::list< std::list<int> > solution = { { 1, 2 }, { 0, 3 } };
    REQUIRE(collection == solution);
}
