#ifndef _LINEAR_SELECT_HPP
#define _LINEAR_SELECT_HPP

#include <vector>

void InsertionSort(std::vector<int>& arr, size_t left, size_t right)
{
    size_t i, j;
    int key;
    for (j = left + 1; j <= right; ++j)
    {
        key = arr[j];
        i = j - 1;
        while (i >= left && arr[i] > key)
        {
            arr[i + 1] = arr[i];
            --i;
        }
        arr[i + 1] = key;
    }
}

/**
 * Partition the array arround pivot 
 * and group all elements with the value of partition_arround_val together.
 * @param arr input array
 * @param left left-most element index
 * @param right right-most element index
 * @param partition_arround_val pivot value
 * @return close interval [a,b] of elements with the value of partition_arround_val;
 *      if does not exist element with the value of partition_arround_val,
 *      then b is the index of the element that is smaller than partition_arround_val
 *      with the greatest index, and a = b + 1
 */
std::pair<size_t, size_t> Partition(std::vector<int>& arr, 
    size_t left, size_t right, int partition_arround_val)
{
    size_t i, j, m;
    i = left - 1;
    m = i;
    for (j = left; j <= right; ++j)
    {
        if (arr[j] < partition_arround_val)
        {
            ++m;
            ++i;
            std::swap(arr[m], arr[j]);
            std::swap(arr[i], arr[m]);
        }
        else if (arr[j] == partition_arround_val)
        {
            ++m;
            std::swap(arr[m], arr[j]);
        }
    }
    return std::make_pair(i + 1, m);
}

/**
 * Select the arr to find the element with the specfic rank. 
 * This function will lead to a partition of arr arround the element with the specfic rank.
 * @param arr input array
 * @param left left-most element index
 * @param right right-most element index
 * @param rank start by 0; start count from arr[left]
 * @return close interval [a,b] of elements with the value of element with the specfic rank;
 */
std::pair<size_t, size_t> LinearSelect(std::vector<int>& arr, size_t left, size_t right, size_t rank)
{
    size_t i, n, n_div_five_floor, n_div_five_ceil, n_div_five_floor_mult_five;
    std::vector<int> medians;
    int median_of_medians;
    if (left >= right)
        return std::make_pair(left, right);
    n = right - left + 1;
    n_div_five_floor = n / 5;
    n_div_five_floor_mult_five = n_div_five_floor * 5;
    n_div_five_ceil = (n_div_five_floor_mult_five < n) ? 
        n_div_five_floor + 1 : n_div_five_floor;
    medians.reserve(n_div_five_ceil);
    for (i = left; i < left + n_div_five_floor_mult_five; i += 5)
    {
        InsertionSort(arr, i, i + 4);
        medians.push_back(arr[i + 2]);
    }
    if (n_div_five_floor_mult_five < n)
    {
        InsertionSort(arr, left + n_div_five_floor_mult_five, right);
        medians.push_back(arr[left + n_div_five_floor_mult_five 
            + ((n - 1 - n_div_five_floor_mult_five) >> 1)]);
    }
    median_of_medians = medians[LinearSelect(medians, 0, n_div_five_ceil - 1, 
        (n_div_five_ceil - 1) >> 1).first];
    std::pair<int, int> pivot_interval = Partition(arr, left, right, median_of_medians);
    if (left + rank < pivot_interval.first)
        return LinearSelect(arr, left, pivot_interval.first - 1, rank);
    else if (left + rank > pivot_interval.second)
        return LinearSelect(arr, pivot_interval.second + 1, right, left + rank - pivot_interval.second - 1);
    else
        return pivot_interval;
}

#endif