#include <vector>
#include <list>

using LengthTableRow = std::vector<int>;
using LengthTable = std::vector< LengthTableRow >;

// O(size_x * size_y)
template <typename T>
LengthTable LCSLength
    (const std::vector<T>& sequence_x, const std::vector<T>& sequence_y) 
{
    size_t size_x, size_y, it_x, it_y; 
    int sequence_length_x_prev, sequence_length_y_prev;
    size_x = sequence_x.size();
    size_y = sequence_y.size();
    LengthTable length_table(size_x + 1, LengthTableRow(size_y + 1));
    for (it_x = 0; it_x <= size_x; ++it_x)
        length_table[it_x][0] = 0;
    for (it_y = 1; it_y <= size_y; ++it_y)
        length_table[0][it_y] = 0;
    for (it_x = 1; it_x <= size_x; ++it_x) 
    {
        for (it_y = 1; it_y <= size_y; ++it_y) 
        {
            if (sequence_x[it_x - 1] == sequence_y[it_y - 1]) 
            {
                length_table[it_x][it_y] = 
                    length_table[it_x - 1][it_y - 1] + 1;
            } 
            else 
            {
                sequence_length_x_prev = length_table[it_x - 1][it_y];
                sequence_length_y_prev = length_table[it_x][it_y - 1];
                if (sequence_length_x_prev >= sequence_length_y_prev) 
                {
                    length_table[it_x][it_y] = sequence_length_x_prev;
                } 
                else 
                {
                    length_table[it_x][it_y] = sequence_length_y_prev;
                }
            }
        }
    }
    return length_table;
}

// O(size_x + size_y)
template <typename T>
std::list<T> OutputLCS(LengthTable length_table, 
    const std::vector<T>& sequence_x, const std::vector<T>& sequence_y)
{
    size_t it_x, it_y;
    std::list<T> common_sequence;
    it_x = sequence_x.size();
    it_y = sequence_y.size();
    while (it_x > 0 && it_y > 0)
    {
        const T& element_x = sequence_x[it_x - 1];
        const T& element_y = sequence_y[it_y - 1];
        if (element_x == element_y)
        {
            common_sequence.push_front(element_y);
            --it_x;
            --it_y;
        }
        else if (length_table[it_x - 1][it_y] >= length_table[it_x][it_y - 1])
        {
            --it_x;
        }
        else
        {
            --it_y;
        }
    }
    return common_sequence;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>

TEST_CASE("example")
{
    std::vector<char> sequence_x, sequence_y;
    LengthTable length_table;
    std::list<char> lcs_result, lcs_solution;
    sequence_x = { 'A', 'B', 'C', 'B', 'D', 'A', 'B' };
    sequence_y = { 'B', 'D', 'C', 'A', 'B', 'A' };
    length_table = LCSLength(sequence_x, sequence_y);
    REQUIRE(length_table[7][5] == 4);
    lcs_result = OutputLCS(length_table, sequence_x, sequence_y);
    lcs_solution = { 'B', 'C', 'B', 'A' };
    REQUIRE(lcs_result == lcs_solution);
}
