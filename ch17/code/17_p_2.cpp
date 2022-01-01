#include <algorithm>
#include <list>
#include <stdexcept>
#include <vector>

template <typename T>
class DynamicBinarySearch
{
public:
    using Iterators = std::pair
        <typename std::list< std::vector<T> >::iterator, 
        typename std::vector<T>::iterator>;
    Iterators Search(const T& key);
    void Insert(const T& key);
    void Delete(const Iterators& its);
private:
    std::list< std::vector<T> > arrays_;
};

template <typename T>
typename DynamicBinarySearch<T>::Iterators 
    DynamicBinarySearch<T>::Search(const T& key)
{
    size_t n, i;
    n = arrays_.size();
    for (typename std::list< std::vector<T> >::iterator it = arrays_.begin(); 
        it != arrays_.end(); ++it)
    {
        typename std::vector<T>::iterator sub_it = 
            std::lower_bound(it->begin(), it->end(), key);
        if (sub_it != it->end() && *sub_it == key)
            return std::make_pair(it, sub_it);
    }
    throw std::out_of_range("the container does not have an element "
                            "with the specified key");
}

template <typename T>
void DynamicBinarySearch<T>::Insert(const T& key)
{
    size_t n;
    std::vector<T> merged_arr, tmp;
    typename std::list< std::vector<T> >::iterator it = arrays_.begin();
    n = 1;
    merged_arr.push_back(key);
    while (it != arrays_.end() && it->size() > 0)
    {
        n >>= 1;
        tmp = std::move(merged_arr);
        merged_arr.reserve(n);
        std::merge(tmp.begin(), tmp.end(), 
            it->begin(), it->end(), 
            std::back_inserter(merged_arr));
        it->clear();
        ++it;
    }
    if (it != arrays_.end())
        *it = std::move(merged_arr);
    else
        arrays_.emplace_back(std::move(merged_arr));
}

template <typename T>
void DynamicBinarySearch<T>::Delete(const Iterators& its)
{
    size_t n;
    // find the full array with the smallest size 
    // (i.e. the full array with smallest index in arrays_)
    typename std::list< std::vector<T> >::iterator 
        first_full_it = arrays_.begin();
    while (first_full_it->size() == 0)
    {
        ++first_full_it;
    }
    // delete the element and refill the array with first_full_it->back()
    typename std::vector<T>::iterator processing_element_it = its.second,
        target_element_it = std::lower_bound
        (its.first->begin(), its.first->end(), first_full_it->back());
    if (target_element_it > processing_element_it)
    {
        ++processing_element_it;
        while (target_element_it != processing_element_it)
        {
            *(processing_element_it - 1) = std::move(*processing_element_it);
            ++processing_element_it;
        }
        *(processing_element_it - 1) = std::move(first_full_it->back());
    }
    else
    {
        while (target_element_it != processing_element_it)
        {
            *processing_element_it = std::move(*(processing_element_it - 1));
            --processing_element_it;
        }
        *processing_element_it = std::move(first_full_it->back());
    }
    // split first_full_it and assign them into arrays with smaller index in arrays_
    n = 1;
    typename std::vector<T>::iterator element_it = first_full_it->begin();
    for (typename std::list< std::vector<T> >::iterator it = arrays_.begin();
        it != first_full_it; ++it)
    {
        it->assign(std::move_iterator(element_it), 
            std::move_iterator(element_it) + n);
        element_it = element_it + n;
        n <<= 1;
    }
    first_full_it->clear();
    // remove empty arrays (optional)
    if (arrays_.back().empty())
    {
        arrays_.pop_back();
    }
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

template <typename T>
void CheckSearch(DynamicBinarySearch<T>& dbs, const T& key)
{
    typename DynamicBinarySearch<T>::Iterators its = 
        dbs.Search(key);
    assert(*(its.second) == key);
    assert((*(its.first))[std::distance(its.first->begin(), its.second)]
        == key);
}

TEST_CASE("Search and Insert")
{
    int i, j, h;
    std::cout << "-------------------\n";
    DynamicBinarySearch<int> dbs;
    clock_t start = clock();
    dbs.Insert(8);
    dbs.Insert(2);
    dbs.Insert(14);
    CheckSearch(dbs, 8);
    CheckSearch(dbs, 2);
    CheckSearch(dbs, 14);
    dbs.Insert(11);
    dbs.Insert(1);
    dbs.Insert(4);
    CheckSearch(dbs, 8);
    CheckSearch(dbs, 2);
    CheckSearch(dbs, 14);
    CheckSearch(dbs, 11);
    CheckSearch(dbs, 1);
    CheckSearch(dbs, 4);
    dbs.Insert(15);
    dbs.Insert(7);
    dbs.Insert(5);
    CheckSearch(dbs, 8);
    CheckSearch(dbs, 2);
    CheckSearch(dbs, 14);
    CheckSearch(dbs, 11);
    CheckSearch(dbs, 1);
    CheckSearch(dbs, 4);
    CheckSearch(dbs, 15);
    CheckSearch(dbs, 7);
    CheckSearch(dbs, 5);
    REQUIRE_THROWS(dbs.Search(10));
    REQUIRE_THROWS(dbs.Search(20));
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
}

TEST_CASE("Delete 1")
{
    int i, j, h;
    std::cout << "-------------------\n";
    DynamicBinarySearch<int> dbs;
    clock_t start = clock();
    dbs.Insert(8);
    dbs.Insert(2);
    dbs.Insert(14);
    dbs.Insert(11);
    dbs.Insert(1);
    dbs.Insert(4);
    dbs.Insert(15);
    dbs.Insert(7);
    dbs.Insert(5);
    dbs.Delete(dbs.Search(5));
    REQUIRE_THROWS(dbs.Search(5));
    CheckSearch(dbs, 8);
    CheckSearch(dbs, 2);
    CheckSearch(dbs, 14);
    CheckSearch(dbs, 11);
    CheckSearch(dbs, 1);
    CheckSearch(dbs, 4);
    CheckSearch(dbs, 15);
    CheckSearch(dbs, 7);
    dbs.Delete(dbs.Search(15));
    REQUIRE_THROWS(dbs.Search(5));
    REQUIRE_THROWS(dbs.Search(15));
    CheckSearch(dbs, 8);
    CheckSearch(dbs, 2);
    CheckSearch(dbs, 14);
    CheckSearch(dbs, 11);
    CheckSearch(dbs, 1);
    CheckSearch(dbs, 4);
    CheckSearch(dbs, 7);
    dbs.Delete(dbs.Search(14));
    REQUIRE_THROWS(dbs.Search(14));
    REQUIRE_THROWS(dbs.Search(5));
    REQUIRE_THROWS(dbs.Search(15));
    CheckSearch(dbs, 8);
    CheckSearch(dbs, 2);
    CheckSearch(dbs, 11);
    CheckSearch(dbs, 1);
    CheckSearch(dbs, 4);
    CheckSearch(dbs, 7);
    dbs.Delete(dbs.Search(1));
    REQUIRE_THROWS(dbs.Search(1));
    REQUIRE_THROWS(dbs.Search(14));
    REQUIRE_THROWS(dbs.Search(5));
    REQUIRE_THROWS(dbs.Search(15));
    CheckSearch(dbs, 8);
    CheckSearch(dbs, 11);
    CheckSearch(dbs, 2);
    CheckSearch(dbs, 4);
    CheckSearch(dbs, 7);
    dbs.Delete(dbs.Search(7));
    REQUIRE_THROWS(dbs.Search(7));
    REQUIRE_THROWS(dbs.Search(1));
    REQUIRE_THROWS(dbs.Search(14));
    REQUIRE_THROWS(dbs.Search(5));
    REQUIRE_THROWS(dbs.Search(15));
    CheckSearch(dbs, 8);
    CheckSearch(dbs, 11);
    CheckSearch(dbs, 2);
    CheckSearch(dbs, 4);
    dbs.Delete(dbs.Search(2));
    REQUIRE_THROWS(dbs.Search(2));
    REQUIRE_THROWS(dbs.Search(7));
    REQUIRE_THROWS(dbs.Search(1));
    REQUIRE_THROWS(dbs.Search(14));
    REQUIRE_THROWS(dbs.Search(5));
    REQUIRE_THROWS(dbs.Search(15));
    CheckSearch(dbs, 8);
    CheckSearch(dbs, 11);
    CheckSearch(dbs, 4);
    dbs.Delete(dbs.Search(8));
    REQUIRE_THROWS(dbs.Search(8));
    REQUIRE_THROWS(dbs.Search(2));
    REQUIRE_THROWS(dbs.Search(7));
    REQUIRE_THROWS(dbs.Search(1));
    REQUIRE_THROWS(dbs.Search(14));
    REQUIRE_THROWS(dbs.Search(5));
    REQUIRE_THROWS(dbs.Search(15));
    CheckSearch(dbs, 11);
    CheckSearch(dbs, 4);
    dbs.Delete(dbs.Search(4));
    REQUIRE_THROWS(dbs.Search(4));
    REQUIRE_THROWS(dbs.Search(8));
    REQUIRE_THROWS(dbs.Search(2));
    REQUIRE_THROWS(dbs.Search(7));
    REQUIRE_THROWS(dbs.Search(1));
    REQUIRE_THROWS(dbs.Search(14));
    REQUIRE_THROWS(dbs.Search(5));
    REQUIRE_THROWS(dbs.Search(15));
    CheckSearch(dbs, 11);
    dbs.Delete(dbs.Search(11));
    REQUIRE_THROWS(dbs.Search(11));
    REQUIRE_THROWS(dbs.Search(4));
    REQUIRE_THROWS(dbs.Search(8));
    REQUIRE_THROWS(dbs.Search(2));
    REQUIRE_THROWS(dbs.Search(7));
    REQUIRE_THROWS(dbs.Search(1));
    REQUIRE_THROWS(dbs.Search(14));
    REQUIRE_THROWS(dbs.Search(5));
    REQUIRE_THROWS(dbs.Search(15));
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
}

TEST_CASE("Delete 2")
{
    int i, j, h;
    std::cout << "-------------------\n";
    DynamicBinarySearch<int> dbs;
    clock_t start = clock();
    dbs.Insert(8);
    dbs.Insert(2);
    dbs.Insert(14);
    dbs.Insert(11);
    dbs.Insert(1);
    dbs.Insert(4);
    dbs.Insert(15);
    dbs.Insert(7);
    dbs.Insert(5);
    dbs.Delete(dbs.Search(5));
    REQUIRE_THROWS(dbs.Search(5));
    CheckSearch(dbs, 8);
    CheckSearch(dbs, 2);
    CheckSearch(dbs, 14);
    CheckSearch(dbs, 11);
    CheckSearch(dbs, 1);
    CheckSearch(dbs, 4);
    CheckSearch(dbs, 15);
    CheckSearch(dbs, 7);
    dbs.Delete(dbs.Search(1));
    REQUIRE_THROWS(dbs.Search(5));
    REQUIRE_THROWS(dbs.Search(1));
    CheckSearch(dbs, 8);
    CheckSearch(dbs, 2);
    CheckSearch(dbs, 14);
    CheckSearch(dbs, 11);
    CheckSearch(dbs, 15);
    CheckSearch(dbs, 4);
    CheckSearch(dbs, 7);
    dbs.Delete(dbs.Search(14));
    REQUIRE_THROWS(dbs.Search(14));
    REQUIRE_THROWS(dbs.Search(5));
    REQUIRE_THROWS(dbs.Search(1));
    CheckSearch(dbs, 8);
    CheckSearch(dbs, 2);
    CheckSearch(dbs, 11);
    CheckSearch(dbs, 15);
    CheckSearch(dbs, 4);
    CheckSearch(dbs, 7);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
}

TEST_CASE("Delete 3")
{
    int i, j, h;
    std::cout << "-------------------\n";
    DynamicBinarySearch<int> dbs;
    clock_t start = clock();
    dbs.Insert(8);
    dbs.Insert(2);
    dbs.Insert(14);
    dbs.Insert(11);
    dbs.Insert(1);
    dbs.Insert(4);
    dbs.Insert(15);
    dbs.Insert(7);
    dbs.Insert(5);
    dbs.Delete(dbs.Search(5));
    REQUIRE_THROWS(dbs.Search(5));
    CheckSearch(dbs, 8);
    CheckSearch(dbs, 2);
    CheckSearch(dbs, 14);
    CheckSearch(dbs, 11);
    CheckSearch(dbs, 1);
    CheckSearch(dbs, 4);
    CheckSearch(dbs, 15);
    CheckSearch(dbs, 7);
    dbs.Delete(dbs.Search(7));
    REQUIRE_THROWS(dbs.Search(5));
    REQUIRE_THROWS(dbs.Search(7));
    CheckSearch(dbs, 8);
    CheckSearch(dbs, 2);
    CheckSearch(dbs, 14);
    CheckSearch(dbs, 11);
    CheckSearch(dbs, 15);
    CheckSearch(dbs, 4);
    CheckSearch(dbs, 1);
    dbs.Delete(dbs.Search(15));
    REQUIRE_THROWS(dbs.Search(15));
    REQUIRE_THROWS(dbs.Search(5));
    REQUIRE_THROWS(dbs.Search(7));
    CheckSearch(dbs, 8);
    CheckSearch(dbs, 2);
    CheckSearch(dbs, 11);
    CheckSearch(dbs, 14);
    CheckSearch(dbs, 4);
    CheckSearch(dbs, 1);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
}
