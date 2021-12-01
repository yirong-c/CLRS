#include <cmath>
#include <ctime>
#include <limits.h>
#include <vector>

inline int GetRangedRandomNum(int left_end, int right_end)
{
	srand((unsigned)time(NULL));
	return rand() % (right_end - left_end + 1) + left_end;
}

struct Point
{
    int x;
    int y;
};

inline int Distance(const Point& a, const Point& b)
{
    int diff_x, diff_y;
    diff_x = std::abs(a.x - b.x);
    diff_y = std::abs(a.y - b.y);
    return diff_x * diff_x + diff_y * diff_y;
}

int Partition(std::vector<Point>& points, int left, int right)
{
    int pivot;
    int i, j;
    std::swap(points[right], points[GetRangedRandomNum(left, right)]);
    pivot = points[right].x;
    i = left - 1;
    for (j = left; j < right; ++j)
    {
        if (points[j].x <= pivot)
        {
            ++i;
            std::swap(points[i], points[j]);
        }
    }
    ++i;
    std::swap(points[i], points[right]);
    return i;
}

void QuickSort(std::vector<Point>& points, int left, int right)
{
    int pivot_index;
    if (left < right)
    {
        pivot_index = Partition(points, left, right);
        QuickSort(points, left, pivot_index - 1);
        QuickSort(points, pivot_index + 1, right);
    }
}

using Row = std::vector<int>;
using Table = std::vector<Row>;

// O(n^2)
std::vector<size_t> BitonicTour(std::vector<Point>& points)
{
    int l_length;
    size_t size, i, j, b, result_left_p, result_right_p;
    size = points.size();
    Table t_length(size, Row(size, INT_MAX)),
        t_left_neighbour(size, Row(size));
    // r_neighbour_distance[j] == Distance(points[j - 1], points[j])
    Row r_neighbour_distance(size);
    std::vector<size_t> result(size);
    // sort points by x-coordinate
    QuickSort(points, 0, (int)size - 1);
    // compute
    for (j = 2; j < size - 1; ++j)
    {
        r_neighbour_distance[j] = Distance(points[j - 1], points[j]);
    }
    t_length[0][1] = Distance(points[0], points[1]);
    t_left_neighbour[0][1] = 0;
    for (i = 0; i < size - 1; ++i)
    {
        j = i + 1;
        for (b = 0; b < i; ++b)
        {
            l_length = t_length[b][i] + Distance(points[b], points[j]);
            if (l_length < t_length[i][j])
            {
                t_length[i][j] = l_length;
                t_left_neighbour[i][j] = b;
            }
        }
        for (j = i + 2; j < size - 1; ++j)
        {
            t_length[i][j] = t_length[i][j - 1] + r_neighbour_distance[j];
            t_left_neighbour[i][j] = j - 1;
        }
    }
    // output
    result_left_p = 0;
    result_right_p = size - 1;
    i = size - 2;
    j = size - 1;
    while (i != j)
    {
        if (i < j)
        {
            result[result_left_p] = j;
            ++result_left_p;
            j = t_left_neighbour[i][j];
        }
        else
        {
            result[result_right_p] = i;
            --result_right_p;
            i = t_left_neighbour[j][i];
        }
    }
    result[result_left_p] = 0;
    return result;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("Example")
{
    std::vector<Point> points = {
        { 6, 5 }, { 5, 2 }, { 0, 0 }, { 2, 3 },
        { 7, 1 }, { 8, 4 }, { 1, 6 }
    };
    clock_t start = clock();
    std::vector<size_t> result = BitonicTour(points);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::vector<size_t> solution = { 6, 4, 1, 0, 2, 3, 5 };
    REQUIRE(result == solution);
}
