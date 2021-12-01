#include <string>
#include <vector>
#include <limits.h>

using LengthTableRow = std::vector<int>;
using LengthTable = std::vector<LengthTableRow>;
enum PointerType { COMMON, I_SUCC, J_PREV, NIL = -1 };
using PointerTableRow = std::vector<PointerType>;
using PointerTable = std::vector<PointerTableRow>;

// LPS: Longest Palindrome Subsequence

// O(n^2)
std::string LPS(const std::string& str) 
{
    size_t size, i, j, lps_left, lps_right;
    size = str.size();
    std::string result;
    LengthTable lengths(size, LengthTableRow(size, INT_MIN));
    PointerTable pointers(size, PointerTableRow(size, NIL));
    // compute
    for (i = 0; i < size - 1; ++i)
    {
        lengths[i][i] = 1;
        j = i + 1;
        if (str[i] == str[j])
        {
            lengths[i][j] = 2;
            pointers[i][j] = COMMON;
        }
        else
        {
            lengths[i][j] = 1;
            pointers[i][j] = I_SUCC;
        }
    }
    lengths[i][i] = 1;// i == size - 1
    for (i = size - 3; i < size; --i)
    {
        for (j = i + 2; j < size; ++j)
        {
            if (str[i] == str[j])
            {
                lengths[i][j] = lengths[i + 1][j - 1] + 2;
                pointers[i][j] = COMMON;
            }
            else if (lengths[i + 1][j] >= lengths[i][j - 1])
            {
                lengths[i][j] = lengths[i + 1][j];
                pointers[i][j] = I_SUCC;
            }
            else
            {
                lengths[i][j] = lengths[i][j - 1];
                pointers[i][j] = J_PREV;
            }
        }
    }
    // output
    i = 0;
    j = size - 1;
    lps_left = 0;
    lps_right = lengths[i][j] - 1;
    result.resize(lengths[i][j]);
    while (i < j)
    {
        if (pointers[i][j] == I_SUCC)
        {
            ++i;
        }
        else if (pointers[i][j] == J_PREV)
        {
            --j;
        }
        else
        {
            result[lps_left] = str[i];
            result[lps_right] = str[j];
            ++i;
            --j;
            ++lps_left;
            --lps_right;
        }
    }
    if (i == j)
    {
        result[lps_left] = str[i];
    }
    return result;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>

TEST_CASE()
{
    std::string result = LPS("character");
    REQUIRE(result == "carac");
    result = LPS("charracter");
    REQUIRE(result == "carrac");
}
