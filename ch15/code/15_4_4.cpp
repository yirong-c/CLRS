#include <vector>

using LengthTableRow = std::vector<int>;
using LengthTable = std::vector<LengthTableRow>;
enum PointerType { X_Y_PREV, X_PREV, Y_PREV, NIL = -1 };
using PointerTableRow = std::vector<PointerType>;
using PointerTable = std::vector<PointerTableRow>;

// O(size_x * size_y)
template <typename T>
std::pair<int, PointerTable> LCSLengthByTwoRowLengthTable
    (const std::vector<T>& sequence_x, const std::vector<T>& sequence_y) 
{
    size_t size_x, size_y, it_x, it_y;
    int sequence_length_x_prev, sequence_length_y_prev;
    size_x = sequence_x.size();
    size_y = sequence_y.size();
    if (size_x < size_y)
        return LCSLengthByTwoRowLengthTable(sequence_y, sequence_x);
    LengthTableRow length_table_row_1(size_y + 1), length_table_row_2(size_y + 1);
    PointerTable pointer_table(size_x + 1, PointerTableRow(size_y + 1));
    for (it_y = 0; it_y <= size_y; ++it_y)
        length_table_row_1[it_y] = 0;
    for (it_x = 1; it_x <= size_x; ++it_x) 
    {
        for (it_y = 1; it_y <= size_y; ++it_y) 
        {
            if (sequence_x[it_x - 1] == sequence_y[it_y - 1]) 
            {
                length_table_row_2[it_y] = 
                    length_table_row_1[it_y - 1] + 1;
                pointer_table[it_x][it_y] = PointerType::X_Y_PREV;
            } 
            else 
            {
                sequence_length_x_prev = length_table_row_1[it_y];
                sequence_length_y_prev = length_table_row_2[it_y - 1];
                if (sequence_length_x_prev >= sequence_length_y_prev) 
                {
                    length_table_row_2[it_y] = sequence_length_x_prev;
                    pointer_table[it_x][it_y] = PointerType::X_PREV;
                } 
                else 
                {
                    length_table_row_2[it_y] = sequence_length_y_prev;
                    pointer_table[it_x][it_y] = PointerType::Y_PREV;
                }
            }
        }
        length_table_row_1.swap(length_table_row_2);
    }
    return std::make_pair(length_table_row_2[size_y], std::move(pointer_table));
}

// O(size_x * size_y)
template <typename T>
std::pair<int, PointerTable> LCSLengthByOneRowLengthTable
    (const std::vector<T>& sequence_x, const std::vector<T>& sequence_y) 
{
    size_t size_x, size_y, it_x, it_y;
    int sequence_length_x_y_prev, sequence_length_x_y_now;
    size_x = sequence_x.size();
    size_y = sequence_y.size();
    if (size_x < size_y)
        return LCSLengthByOneRowLengthTable(sequence_y, sequence_x);
    LengthTableRow length_table_row(size_y + 1);
    PointerTable pointer_table(size_x + 1, PointerTableRow(size_y + 1));
    for (it_y = 0; it_y <= size_y; ++it_y)
        length_table_row[it_y] = 0;
    for (it_x = 1; it_x <= size_x; ++it_x) 
    {
        sequence_length_x_y_prev = 0;
        for (it_y = 1; it_y <= size_y; ++it_y) 
        {
            sequence_length_x_y_now = length_table_row[it_y];
            if (sequence_x[it_x - 1] == sequence_y[it_y - 1]) 
            {
                length_table_row[it_y] = 
                    sequence_length_x_y_prev + 1;
                pointer_table[it_x][it_y] = PointerType::X_Y_PREV;
            } 
            else 
            {
                if (length_table_row[it_y] >= length_table_row[it_y - 1]) 
                {
                    // length_table_row[it_y] = length_table_row[it_y];
                    pointer_table[it_x][it_y] = PointerType::X_PREV;
                } 
                else 
                {
                    length_table_row[it_y] = length_table_row[it_y - 1];
                    pointer_table[it_x][it_y] = PointerType::Y_PREV;
                }
            }
            sequence_length_x_y_prev = sequence_length_x_y_now;
        }
    }
    return std::make_pair(length_table_row[size_y], std::move(pointer_table));
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>

template <typename T>
bool CheckTable(const T& a, const T& b)
{
    if (a.size() != b.size()) return false;
    auto b_row_it = b.cbegin();
    for (const auto& a_row : a)
    {
        if (a_row.size() != b_row_it->size())
            return false;
        auto b_col_it = b_row_it->cbegin();
        for (const auto& a_col : a_row)
        {
            if (a_col >= 0 && *b_col_it >= 0 && a_col != *b_col_it)
                return false;
            ++b_col_it;
        }
        ++b_row_it;
    }
    return true;
}

TEST_CASE("example")
{
    std::vector<char> sequence_x, sequence_y;
    std::pair<int, PointerTable> result;
    PointerTable pointer_table_solution;
    sequence_x = { 'A', 'B', 'C', 'B', 'D', 'A', 'B' };
    sequence_y = { 'B', 'D', 'C', 'A', 'B', 'A' };
    pointer_table_solution = {
        { NIL, NIL, NIL, NIL, NIL, NIL, NIL},
        { NIL, X_PREV, X_PREV, X_PREV, X_Y_PREV, Y_PREV, X_Y_PREV },
        { NIL, X_Y_PREV, Y_PREV, Y_PREV, X_PREV, X_Y_PREV, Y_PREV },
        { NIL, X_PREV, X_PREV, X_Y_PREV, Y_PREV, X_PREV, X_PREV },
        { NIL, X_Y_PREV, X_PREV, X_PREV, X_PREV, X_Y_PREV, Y_PREV },
        { NIL, X_PREV, X_Y_PREV, X_PREV, X_PREV, X_PREV, X_PREV },
        { NIL, X_PREV, X_PREV, X_PREV, X_Y_PREV, X_PREV, X_Y_PREV },
        { NIL, X_Y_PREV, X_PREV, X_PREV, X_PREV, X_Y_PREV, X_PREV }
    };
    result = LCSLengthByTwoRowLengthTable(sequence_x, sequence_y);
    REQUIRE(result.first == 4);
    REQUIRE(CheckTable(result.second, pointer_table_solution));
    result = LCSLengthByOneRowLengthTable(sequence_x, sequence_y);
    REQUIRE(result.first == 4);
    REQUIRE(CheckTable(result.second, pointer_table_solution));
}
