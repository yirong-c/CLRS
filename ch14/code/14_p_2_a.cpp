#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>

#include <vector>

struct Node
{
    int key;
    Node *next;
};

/**
 * running time: O(mn)
 * caller is responsible to deallocate the return value
 * n, m must greater than 0
 */
std::vector<int>* GetJosephusPermutation(int n, int m)
{
    Node *nodes, **ptr;
    std::vector<int>* result;
    int i;
    /* allocate nodes */
    nodes = new Node[n];
    /* build circular singly linked list */
    for (i = 0; i < n - 1; ++i)
    {
        nodes[i].key = i + 1;
        nodes[i].next = &(nodes[i + 1]);
    }
    nodes[i].key = i + 1;
    nodes[i].next = nodes;// &(nodes[0])
    /* allocate vector */
    result = new std::vector<int>;
    result->reserve(n);
    /* output */
    /* find the m-th key */
    ptr = &(nodes[n - 1].next);
    for (i = 1; i < m; ++i) ptr = &((*ptr)->next);
    while (true)
    {
        /* add the key into the permutation */
        result->push_back((*ptr)->key);
        /* remove the key */
        if (*ptr == (*ptr)->next) break;
        *ptr = (*ptr)->next;
        /* find the next m-th key */
        for (i = 1; i < m; ++i) ptr = &((*ptr)->next);
    }
    /* deallocate nodes */
    delete[] nodes;
    /* return */
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
