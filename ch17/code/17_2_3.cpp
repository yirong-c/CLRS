#include <vector>

struct Counter
{
    int length;
    std::vector<bool> bits;
    int high_order_one;

    Counter(int length) : length(length), 
        bits(length, 0), high_order_one(-1) {}
};

void Increment(Counter& counter)
{
    int i;
    i = 0;
    while (i < counter.length && counter.bits[i] == 1)
    {
        counter.bits[i] = 0;
        ++i;
    }
    if (i < counter.length)
    {
        counter.bits[i] = 1;
        counter.high_order_one = std::max(i, counter.high_order_one);
    }
    else
    {
        // overflow
        counter.high_order_one = -1;
    }
}

void Reset(Counter& counter)
{
    int i;
    for (i = 0; i < counter.length; ++i)
    {
        counter.bits[i] = 0;
    }
    counter.high_order_one = -1;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE()
{
    int i, j, h;
    std::cout << "-------------------\n";
    Counter counter(8);
    std::vector<bool> solution;
    clock_t start = clock();
    solution = { 0, 0, 0, 0, 0, 0, 0, 0 };
    REQUIRE(counter.bits == solution);
    REQUIRE(counter.high_order_one == -1);
    for (i = 1; i <= 255; ++i)
    {
        Increment(counter);
        j = i;
        h = -1;
        for (bool bit : counter.bits)
        {
            if (j)
                ++h;
            REQUIRE(((j & 1) ^ bit) == 0);
            j >>= 1;
        }
        REQUIRE(counter.high_order_one == h);
    }
    solution = { 1, 1, 1, 1, 1, 1, 1, 1 };
    REQUIRE(counter.bits == solution);
    REQUIRE(counter.high_order_one == 7);
    Increment(counter);
    solution = { 0, 0, 0, 0, 0, 0, 0, 0 };
    REQUIRE(counter.bits == solution);
    REQUIRE(counter.high_order_one == -1);
    for (i = 1; i <= 16; ++i)
    {
        Increment(counter);
        j = i;
        h = -1;
        for (bool bit : counter.bits)
        {
            if (j)
                ++h;
            REQUIRE(((j & 1) ^ bit) == 0);
            j >>= 1;
        }
        REQUIRE(counter.high_order_one == h);
    }
    solution = { 0, 0, 0, 0, 1, 0, 0, 0 };
    REQUIRE(counter.bits == solution);
    REQUIRE(counter.high_order_one == 4);
    Reset(counter);
    solution = { 0, 0, 0, 0, 0, 0, 0, 0 };
    REQUIRE(counter.bits == solution);
    REQUIRE(counter.high_order_one == -1);
    for (i = 1; i <= 255; ++i)
    {
        Increment(counter);
        j = i;
        h = -1;
        for (bool bit : counter.bits)
        {
            if (j)
                ++h;
            REQUIRE(((j & 1) ^ bit) == 0);
            j >>= 1;
        }
        REQUIRE(counter.high_order_one == h);
    }
    solution = { 1, 1, 1, 1, 1, 1, 1, 1 };
    REQUIRE(counter.bits == solution);
    REQUIRE(counter.high_order_one == 7);
    Increment(counter);
    solution = { 0, 0, 0, 0, 0, 0, 0, 0 };
    REQUIRE(counter.bits == solution);
    REQUIRE(counter.high_order_one == -1);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
}
