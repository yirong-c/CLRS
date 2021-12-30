#include <vector>
#include "../../ch9/code/linear_select.hpp"

class DataStructure
{
public:
    void Insert(int x);
    void DeleteLargerHalf();
    const std::vector<int>& Get() const;
private:
    std::vector<int> arr_;
};

void DataStructure::Insert(int x)
{
    arr_.push_back(x);
}

void DataStructure::DeleteLargerHalf()
{
    size_t median = arr_.size() >> 1;
    LinearSelect(arr_, 0, arr_.size() - 1, (arr_.size() - 1) >> 1);
    arr_.erase(arr_.begin() + median, arr_.end());
}

const std::vector<int>& DataStructure::Get() const
{
    return arr_;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("Insert")
{
    int i, j, h;
    std::cout << "-------------------\n";
    DataStructure d;
    std::vector<int> arr = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7, 2, 8, 7, 1, 3, 5, 6, 4 };
    clock_t start = clock();
    for (i = 0; i < arr.size(); ++i)
    {
        d.Insert(arr[i]);
    }
    REQUIRE(d.Get() == arr);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
}

TEST_CASE("DeleteLargerHalf 1")
{
    int i, j, h;
    std::cout << "-------------------\n";
    DataStructure d;
    std::vector<int> arr = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7, 2, 8, 7, 1, 3, 5, 6, 4 };
    clock_t start = clock();
    for (i = 0; i < arr.size(); ++i)
    {
        d.Insert(arr[i]);
    }
    d.DeleteLargerHalf();
    std::vector<int> solution = { 1, 2, 1, 2, 3, 3, 4, 4, 5 }; 
    REQUIRE(d.Get() == solution);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
}

TEST_CASE("DeleteLargerHalf 2")
{
    int i, j, h;
    std::cout << "-------------------\n";
    DataStructure d;
    std::vector<int> arr = { 15, 1, 3, 2, 16, 9, 10, 14, 8, 7, 8, 7, 1, 13, 5, 6, 4 };
    clock_t start = clock();
    for (i = 0; i < arr.size(); ++i)
    {
        d.Insert(arr[i]);
    }
    d.DeleteLargerHalf();
    std::vector<int> solution = { 1, 2, 3, 1, 4, 5, 6, 7 }; 
    REQUIRE(d.Get() == solution);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
}

TEST_CASE("DeleteLargerHalf 3")
{
    int i, j, h;
    std::cout << "-------------------\n";
    DataStructure d;
    std::vector<int> arr = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7, 2, 8, 7, 1, 3, 5, 6, 3 };
    clock_t start = clock();
    for (i = 0; i <= 10; ++i)
    {
        d.Insert(arr[i]);
    }
    d.DeleteLargerHalf();
    std::vector<int> solution = { 1, 2, 2, 3, 4 }; 
    REQUIRE(d.Get() == solution);
    for (; i < arr.size(); ++i)
    {
        d.Insert(arr[i]);
    }
    d.DeleteLargerHalf();
    solution = { 1, 2, 2, 1, 3, 3 }; 
    REQUIRE(d.Get() == solution);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
}
