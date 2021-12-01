#include <vector>

using LengthTableRow = std::vector<int>;
using LengthTable = std::vector<LengthTableRow>;
enum PointerType { X_Y_PREV, X_PREV, Y_PREV, NIL = -1 };
using PointerTableRow = std::vector<PointerType>;
using PointerTable = std::vector<PointerTableRow>;

template <typename T>
int RecursiveLCSLengthMemoized
    (const std::vector<T>& sequence_x, const std::vector<T>& sequence_y, 
    size_t it_x, size_t it_y,
    LengthTable& length_table, PointerTable& pointer_table) 
{
    int sequence_length_x_prev, sequence_length_y_prev;
    if (length_table[it_x][it_y] >= 0)
        return length_table[it_x][it_y];
    if (sequence_x[it_x - 1] == sequence_y[it_y - 1])
    {
        length_table[it_x][it_y] = RecursiveLCSLengthMemoized(sequence_x, sequence_y, 
            it_x - 1, it_y - 1, length_table, pointer_table) + 1;
        pointer_table[it_x][it_y] = PointerType::X_Y_PREV;
    }
    else
    {
        sequence_length_x_prev = RecursiveLCSLengthMemoized(sequence_x, sequence_y, 
            it_x - 1, it_y, length_table, pointer_table);
        sequence_length_y_prev = RecursiveLCSLengthMemoized(sequence_x, sequence_y, 
            it_x, it_y - 1, length_table, pointer_table);
        if (sequence_length_x_prev >= sequence_length_y_prev) 
        {
            length_table[it_x][it_y] = sequence_length_x_prev;
            pointer_table[it_x][it_y] = PointerType::X_PREV;
        } 
        else 
        {
            length_table[it_x][it_y] = sequence_length_y_prev;
            pointer_table[it_x][it_y] = PointerType::Y_PREV;
        }
    }
    return length_table[it_x][it_y];
}

template <typename T>
std::pair<LengthTable, PointerTable> LCSLengthMemoized
    (const std::vector<T>& sequence_x, const std::vector<T>& sequence_y) 
{
    size_t size_x, size_y, it_x, it_y, 
        sequence_length_x_prev, sequence_length_y_prev;
    size_x = sequence_x.size();
    size_y = sequence_y.size();
    // note this line init all elements in length_table with -1
    LengthTable length_table(size_x + 1, LengthTableRow(size_y + 1, -1));
    PointerTable pointer_table(size_x + 1, PointerTableRow(size_y + 1, NIL));
    for (it_x = 0; it_x <= size_x; ++it_x)
        length_table[it_x][0] = 0;
    for (it_y = 1; it_y <= size_y; ++it_y)
        length_table[0][it_y] = 0;
    RecursiveLCSLengthMemoized(sequence_x, sequence_y, size_x, size_y, 
        length_table, pointer_table);
    return std::make_pair(std::move(length_table), std::move(pointer_table));
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
    std::pair<LengthTable, PointerTable> result;
    LengthTable length_table_solution;
    PointerTable pointer_table_solution;
    sequence_x = { 'A', 'B', 'C', 'B', 'D', 'A', 'B' };
    sequence_y = { 'B', 'D', 'C', 'A', 'B', 'A' };
    result = LCSLengthMemoized(sequence_x, sequence_y);
    LengthTable& length_table_result = result.first;
    PointerTable& pointer_table_result = result.second;
    length_table_solution = { 
        { 0, 0, 0, 0, 0, 0, 0 }, 
        { 0, 0, 0, 0, 1, 1, 1 }, 
        { 0, 1, 1, 1, 1, 2, 2 }, 
        { 0, 1, 1, 2, 2, 2, 2 }, 
        { 0, 1, 1, 2, 2, 3, 3 }, 
        { 0, 1, 2, 2, 2, 3, 3 }, 
        { 0, 1, 2, 2, 3, 3, 4 }, 
        { 0, 1, 2, 2, 3, 4, 4 } 
    };
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
    REQUIRE(CheckTable(length_table_result, length_table_solution));
    REQUIRE(CheckTable(pointer_table_result, pointer_table_solution));
}
