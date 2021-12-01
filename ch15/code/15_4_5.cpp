#include <vector>

// O(n^2)
std::vector<int> LongestIncreasingSubsequence
    (const std::vector<int>& sequence)
{
    size_t i, j, size, sequence_length_it, max_length_index;
    size = sequence.size();
    std::vector<size_t> length(size), prev(size);
    // compute
    max_length_index = 0;
    for (i = 0; i < size; ++i)
    {
        length[i] = 1;
        for (j = 0; j < i; ++j)
        {
            if (sequence[j] < sequence[i])
            {
                sequence_length_it = length[j] + 1;
                if (sequence_length_it > length[i])
                {
                    length[i] = sequence_length_it;
                    prev[i] = j;
                }
            }
        }
        if (length[i] > length[max_length_index])
            max_length_index = i;
    }
    // output
    std::vector<int> result(length[max_length_index]);
    for (i = length[max_length_index] - 1; i < length[max_length_index]; --i)
    {
        result[i] = sequence[max_length_index];
        max_length_index = prev[max_length_index];
    }
    return result;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>

TEST_CASE("1")
{
    std::vector<int> sequence, lis_result, lis_solution;
    sequence = { 10, 9, 2, 5, 3, 4, 7, 101, 18 };
    SECTION("LongestIncreasingSubsequence")
    {
        lis_result = LongestIncreasingSubsequence(sequence);
        lis_solution = { 2, 3, 4, 7, 101 }; 
        REQUIRE(lis_result == lis_solution);
    }
}

TEST_CASE("2")
{
    std::vector<int> sequence, lis_result, lis_solution;
    sequence = { 4, 10, 4, 3, 8, 9 };
    SECTION("LongestIncreasingSubsequence")
    {
        lis_result = LongestIncreasingSubsequence(sequence);
        lis_solution = { 4, 8, 9 }; 
        REQUIRE(lis_result == lis_solution);
    }
}

TEST_CASE("3")
{
    std::vector<int> sequence, lis_result, lis_solution;
    sequence = { 0, 1, 0, 3, 2, 3 };
    SECTION("LongestIncreasingSubsequence")
    {
        lis_result = LongestIncreasingSubsequence(sequence);
        lis_solution = { 0, 1, 2, 3 }; 
        REQUIRE(lis_result == lis_solution);
    }
}
