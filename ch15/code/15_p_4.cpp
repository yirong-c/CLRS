#include <limits.h>
#include <vector>
#include <list>

// O(n^2)
std::list<int> PrintingNeatly(int line_max_char, 
    const std::vector<int>& lengths)
{
    int size, j, k, extra;
    size = (int)(lengths.size());
    /**
     * min_sum[k + 1] is the minimum extra char
     * from the first word to the word which the length is lengths[k]
     */
    std::vector<int> min_sum(size + 1);
    std::vector<int> line_start_index(size);
    std::list<int> result;
    // compute
    min_sum[0] = 0;
    for (j = 0; j < size - 1; ++j)
    {
        min_sum[j + 1] = INT_MAX;
        extra = line_max_char + 1;
        for (k = j; k >= 0; --k)
        {
            extra = extra - 1 - lengths[k];
            if (extra < 0)
                break;
            if (min_sum[k] + extra < min_sum[j + 1])
            {
                min_sum[j + 1] = min_sum[k] + extra;
                line_start_index[j] = k;
            }
        }
    }
    extra = line_max_char + 1;
    min_sum[size] = INT_MAX;
    for (k = size - 1; k >= 0; --k)
    {
        extra = extra - 1 - lengths[k];
        if (extra < 0)
            break;
        if (min_sum[k] < min_sum[size])
        {
            min_sum[size] = min_sum[k];
            line_start_index[size - 1] = k;
        }
    }
    // output
    k = size;
    while (k != 0)
    {
        k = line_start_index[k - 1];
        result.push_front(k);
    }
    return result;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("1")
{
    std::cout << "------------------------" << std::endl;
    std::vector<int> lengths = { 4, 6, 1, 5, 7, 3, 2, 8, 5, 3, 9, 2, 4, 5 };
    clock_t start = clock();
    std::list<int> result = PrintingNeatly(30, lengths);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: ";
    for (int i : result)
        std::cout << i << " ";
    std::cout << std::endl;
    std::list<int> solution = { 0, 5, 10 };
    REQUIRE(result == solution);
}

TEST_CASE("2")
{
    std::cout << "------------------------" << std::endl;
    std::vector<int> lengths = { 4, 6, 1, 5, 7, 3 };
    clock_t start = clock();
    std::list<int> result = PrintingNeatly(30, lengths);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: ";
    for (int i : result)
        std::cout << i << " ";
    std::cout << std::endl;
    std::list<int> solution = { 0, 5 };
    REQUIRE(result == solution);
}

TEST_CASE("3")
{
    std::cout << "------------------------" << std::endl;
    std::vector<int> lengths = { 4, 6, 1, 5, 7 };
    clock_t start = clock();
    std::list<int> result = PrintingNeatly(30, lengths);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: ";
    for (int i : result)
        std::cout << i << " ";
    std::cout << std::endl;
    std::list<int> solution = { 0 };
    REQUIRE(result == solution);
}

TEST_CASE("4")
{
    std::cout << "------------------------" << std::endl;
    std::vector<int> lengths = { 4 };
    clock_t start = clock();
    std::list<int> result = PrintingNeatly(30, lengths);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: ";
    for (int i : result)
        std::cout << i << " ";
    std::cout << std::endl;
    std::list<int> solution = { 0 };
    REQUIRE(result == solution);
}
