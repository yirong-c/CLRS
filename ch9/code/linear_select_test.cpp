#include "linear_select.hpp"

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("InsertionSort 1")
{
    int i, j, h;
    std::cout << "-------------------\n";
    std::vector<int> arr = { 5, 2, 4, 6, 1, 3 };
    clock_t start = clock();
    InsertionSort(arr, 0, 5);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
    std::vector<int> solution = { 1, 2, 3, 4, 5, 6 };
    REQUIRE(arr == solution);
}

TEST_CASE("InsertionSort 2")
{
    int i, j, h;
    std::cout << "-------------------\n";
    std::vector<int> arr = { 5, 2, 4, 6, 1, 3 };
    clock_t start = clock();
    InsertionSort(arr, 1, 4);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
    std::vector<int> solution = { 5, 1, 2, 4, 6, 3 };
    REQUIRE(arr == solution);
}

TEST_CASE("Partition 1")
{
    int i, j, h;
    std::cout << "-------------------\n";
    std::vector<int> arr = { 2, 8, 7, 1, 3, 5, 6, 4 };
    clock_t start = clock();
    std::pair<size_t, size_t> interval = Partition(arr, 0, 7, 4);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
    std::vector<int> solution = { 2, 1, 3, 4, 7, 5, 6, 8 };
    std::pair<size_t, size_t> interval_solution = { 3, 3 };
    REQUIRE(arr == solution);
    REQUIRE(interval == interval_solution);
}

TEST_CASE("Partition 2")
{
    int i, j, h;
    std::cout << "-------------------\n";
    std::vector<int> arr = { 2, 8, 7, 1, 3, 5, 6, 4 };
    clock_t start = clock();
    std::pair<size_t, size_t> interval = Partition(arr, 2, 5, 4);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
    std::vector<int> solution = { 2, 8, 1, 3, 7, 5, 6, 4 };
    std::pair<size_t, size_t> interval_solution = { 4, 3 };
    REQUIRE(arr == solution);
    REQUIRE(interval == interval_solution);
}

TEST_CASE("Partition 3")
{
    int i, j, h;
    std::cout << "-------------------\n";
    std::vector<int> arr = { 2, 8, 5, 1, 3, 5, 6, 4 };
    clock_t start = clock();
    std::pair<size_t, size_t> interval = Partition(arr, 1, 6, 5);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
    std::vector<int> solution = { 2, 1, 3, 5, 5, 8, 6, 4 };
    std::pair<size_t, size_t> interval_solution = { 3, 4 };
    REQUIRE(arr == solution);
    REQUIRE(interval == interval_solution);
}

TEST_CASE("LinearSelect 1")
{
    int i, j, h;
    std::cout << "-------------------\n";
    std::vector<int> arr = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7, 2, 8, 7, 1, 3, 5, 6, 4 };
    clock_t start = clock();
    std::pair<size_t, size_t> interval = LinearSelect(arr, 0, 17, 8);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
    std::vector<int> solution = { 1, 2, 1, 2, 3, 3, 4, 4, 5, 6, 7, 7, 9, 10, 8, 16, 14, 8 };
    std::pair<size_t, size_t> interval_solution = { 8, 8 };
    REQUIRE(arr == solution);
    REQUIRE(interval == interval_solution);
}

TEST_CASE("LinearSelect 2")
{
    int i, j, h;
    std::cout << "-------------------\n";
    std::vector<int> arr = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7, 2, 4, 7, 1, 3, 6, 4 };
    clock_t start = clock();
    std::pair<size_t, size_t> interval = LinearSelect(arr, 0, 16, 8);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
    std::vector<int> solution = { 1, 2, 1, 2, 3, 3, 4, 4, 4, 6, 7, 7, 9, 10, 14, 16, 8 };
    std::pair<size_t, size_t> interval_solution = { 6, 8 };
    REQUIRE(arr == solution);
    REQUIRE(interval == interval_solution);
}

TEST_CASE("LinearSelect 3")
{
    int i, j, h;
    std::cout << "-------------------\n";
    std::vector<int> arr = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7, 2, 8, 7, 1, 3, 5, 6, 4 };
    clock_t start = clock();
    std::pair<size_t, size_t> interval = LinearSelect(arr, 0, 17, 3);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
    std::vector<int> solution = { 1, 1, 2, 2, 3, 3, 8, 9, 10, 14, 4, 16, 7, 7, 8, 4, 5, 6 };
    std::pair<size_t, size_t> interval_solution = { 2, 3 };
    REQUIRE(arr == solution);
    REQUIRE(interval == interval_solution);
}

TEST_CASE("LinearSelect 4")
{
    int i, j, h;
    std::cout << "-------------------\n";
    std::vector<int> arr = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7, 2, 8, 7, 1, 3, 5, 6, 4 };
    clock_t start = clock();
    std::pair<size_t, size_t> interval = LinearSelect(arr, 0, 17, 15);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
    std::vector<int> solution = { 1, 2, 1, 2, 3, 3, 4, 4, 5, 6, 7, 7, 8, 8, 9, 10, 14, 16 };
    std::pair<size_t, size_t> interval_solution = { 15, 15 };
    REQUIRE(arr == solution);
    REQUIRE(interval == interval_solution);
}
