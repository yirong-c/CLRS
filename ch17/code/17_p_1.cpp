#include <vector>

// Assume this function runs in \Theta(k) time
size_t Rev(size_t k, size_t a)
{
    size_t i, rev;
    rev = 0;
    for (i = 0; i < k; ++i)
    {
        rev = (rev << 1) + (a & 1);
        a = a >> 1;
    }
    return rev;
}

// arr.size() must be 2^k for some nonnegative integer k
template <typename T>
void BitReversal(std::vector<T>& arr)
{
    size_t k, tmp, i, n, rev;
    n = arr.size();
    std::vector<bool> counter(n, false);
    tmp = n;
    k = -1;
    while (tmp)
    {
        tmp = tmp >> 1;
        ++k;
    }
    for (i = 0; i < n; ++i)
    {
        if (counter[i] == false)
        {
            rev = Rev(k, i);
            std::swap(arr[i], arr[rev]);
            counter[i] = true;
            counter[rev] = true;
        }
    }
}

size_t BitReversedIncrement(size_t k, size_t a)
{
    size_t i;
    i = 1 << (k - 1);
    while (i > 0 && (a & i) != 0)
    {
        a = a & ( ~ i );
        i = i >> 1;
    }
    a = a | i;
    return a;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("Rev")
{
    int i, j, h;
    std::cout << "-------------------\n";
    clock_t start = clock();
    REQUIRE(Rev(4, 0) == 0);
    REQUIRE(Rev(4, 1) == 8);
    REQUIRE(Rev(4, 2) == 4);
    REQUIRE(Rev(4, 3) == 12);
    REQUIRE(Rev(4, 4) == 2);
    REQUIRE(Rev(4, 5) == 10);
    REQUIRE(Rev(4, 6) == 6);
    REQUIRE(Rev(4, 7) == 14);
    REQUIRE(Rev(4, 8) == 1);
    REQUIRE(Rev(4, 9) == 9);
    REQUIRE(Rev(4, 10) == 5);
    REQUIRE(Rev(4, 11) == 13);
    REQUIRE(Rev(4, 12) == 3);
    REQUIRE(Rev(4, 13) == 11);
    REQUIRE(Rev(4, 14) == 7);
    REQUIRE(Rev(4, 15) == 15);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
}

TEST_CASE("BitReversal")
{
    int i, j, h;
    std::cout << "-------------------\n";
    std::vector<int> arr = { 0, 1, 2, 3, 4, 5, 6, 7, 
        8, 9, 10, 11, 12, 13, 14, 15 };
    clock_t start = clock();
    BitReversal(arr);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
    std::vector<int> solution = { 0, 8, 4, 12, 2, 10, 6, 14, 
        1, 9, 5, 13, 3, 11, 7, 15 };
    REQUIRE(arr == solution);
}

TEST_CASE("BitReversedIncrement")
{
    int i, j, h;
    std::cout << "-------------------\n";
    std::vector<int> result(16);
    clock_t start = clock();
    result[0] = 0;
    for (i = 1; i < 16; ++i)
    {
        result[i] = BitReversedIncrement(4, result[i - 1]);
    }
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
    std::vector<int> solution = { 0, 8, 4, 12, 2, 10, 6, 14, 
        1, 9, 5, 13, 3, 11, 7, 15 };
    REQUIRE(result == solution);
}
