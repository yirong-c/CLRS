#include <vector>

struct Node
{
    int p;
    int rank;
    // set info:
    int subsequence_index_lower;// only root
    int subsequence_index_upper;// only root
    int prev;// only root
    int next;// only root
};

int FindSet(std::vector<Node>& forest, int x)
{
    if (forest[x].p != x)
        forest[x].p = FindSet(forest, forest[x].p);
    return forest[x].p;
}

// keep y's set info
void Link(std::vector<Node>& forest, int x, int y)
{
    forest[forest[x].prev].next = forest[x].next;
    forest[forest[x].next].prev = forest[x].prev;
    if (forest[x].rank > forest[y].rank)
    {
        forest[y].p = x;
        forest[x].subsequence_index_lower = forest[y].subsequence_index_lower;
        forest[x].subsequence_index_upper = forest[y].subsequence_index_upper;
        forest[x].prev = forest[y].prev;
        forest[x].next = forest[y].next;
        forest[forest[x].prev].next = x;
        forest[forest[x].next].prev = x;
    }
    else
    {
        forest[x].p = y;
        if (forest[x].rank == forest[y].rank)
            ++forest[y].rank;
    }
}

void Union(std::vector<Node>& forest, int x, int y)
{
    return Link(forest, FindSet(forest, x), FindSet(forest, y));
}

// operations: E represents by -1
// n: domain size
std::vector<int> OffLineMinimum(const std::vector<int>& operations, int n)
{
    int i, j, m, root, last_root;
    std::vector<Node> forest(n + 1);// index start by 1
    // init disjoin-set forest
    for (i = 1; i <= n; ++i)
    {
        forest[i].p = i;
        forest[i].rank = 0;
    }
    // init subsequence
    m = 1;
    root = 0;
    last_root = 0;
    for (i = 0; i < operations.size(); ++i)
    {
        if (operations[i] < 0)
        {
            // extract
            forest[root].subsequence_index_upper = m;
            ++m;
            last_root = root;
        }
        else if (last_root == root)
        {
            // first insert in the subsequence
            root = operations[i];
            forest[root].subsequence_index_lower = m;
            forest[root].prev = last_root;
            forest[last_root].next = root;
        }
        else
        {
            // non-first insert in the subsequence
            forest[operations[i]].p = root;
            forest[root].rank = 1;
        }
    }
    m = forest[last_root].subsequence_index_upper;
    forest[last_root].next = 0;// for situation of that the last operation is extract
    forest[0].subsequence_index_lower = m + 1;
    // compute extracted array
    std::vector<int> extracted(m);
    for (i = 1; i <= n; ++i)
    {
        root = FindSet(forest, i);
        j = forest[root].subsequence_index_lower;
        if (j <= m)
        {
            extracted[j - 1] = i;
            if (forest[root].subsequence_index_lower < forest[root].subsequence_index_upper)
                ++forest[root].subsequence_index_lower;
            else
                Union(forest, root, forest[root].next);
        }
    }
    return extracted;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("example")
{
    std::cout << "-------------------\n";
    std::vector<int> operations = { 4, 8, -1, 3, -1, 9, 2, 6, -1, -1, -1, 1, 7, -1, 5 };
    std::vector<int> extracted = OffLineMinimum(operations, 9);
    std::vector<int> solution = { 4, 3, 2, 6, 8, 1 };
    REQUIRE(extracted == solution);
}

TEST_CASE("2")
{
    std::cout << "-------------------\n";
    std::vector<int> operations = { 4, 8, -1, 3, -1, 9, 2, 6, -1, -1, -1, 1, 7, -1, -1, 5 };
    std::vector<int> extracted = OffLineMinimum(operations, 9);
    std::vector<int> solution = { 4, 3, 2, 6, 8, 1, 7 };
    REQUIRE(extracted == solution);
}

TEST_CASE("3")
{
    std::cout << "-------------------\n";
    std::vector<int> operations = { 4, 8, -1, 3, -1, 5, 2, 6, -1, -1, -1, 1, 7, -1 };
    std::vector<int> extracted = OffLineMinimum(operations, 8);
    std::vector<int> solution = { 4, 3, 2, 5, 6, 1 };
    REQUIRE(extracted == solution);
}

TEST_CASE("4")
{
    std::cout << "-------------------\n";
    std::vector<int> operations = { 4, 8, -1, 3, -1, 5, 2, 6, -1, -1, -1, 1, 7, -1, -1 };
    std::vector<int> extracted = OffLineMinimum(operations, 8);
    std::vector<int> solution = { 4, 3, 2, 5, 6, 1, 7 };
    REQUIRE(extracted == solution);
}

TEST_CASE("5")
{
    std::cout << "-------------------\n";
    std::vector<int> operations = { 4, 8, -1, -1, 3, -1, 5, 2, 6, -1, -1, -1, 1, 7, -1, -1 };
    std::vector<int> extracted = OffLineMinimum(operations, 8);
    std::vector<int> solution = { 4, 8, 3, 2, 5, 6, 1, 7 };
    REQUIRE(extracted == solution);
}
