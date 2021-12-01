#include <vector>

// O(nlgn)
std::vector<int> LongestIncreasingSubsequenceBinarySerach
    (const std::vector<int>& sequence)
{
    size_t i, size, seq_index;
    int lower, upper, middle;
    size = sequence.size();
    /**
     * prev
     * index is the element index of sequence
     * value is the element index of sequence 
     *      such that the element is the prev element 
     *      in the increasing subsequence
     * ---------------------------------------------------
     * sub_seq
     * index is length of the increasing subsequence
     * value is the element index of sequence
     */
    std::vector<size_t> prev(size), sub_seq;
    sub_seq.reserve(size);
    // compute
    for (i = 0; i < size; ++i)
    {
        lower = 0;
        upper = sub_seq.size() - 1;
        while (lower <= upper)
        {
            middle = lower + ((upper - lower) >> 1);
            if (sequence[sub_seq[middle]] > sequence[i])
            {
                upper = middle - 1;
            }
            else if (sequence[sub_seq[middle]] < sequence[i])
            {
                lower = middle + 1;
            }
            else
            {
                lower = middle;
                break;
            }
        }
        // sequence[i] <= sequence[sub_seq[lower]] must be true
        if (lower >= sub_seq.size())
            sub_seq.push_back(i);
        else
            sub_seq[lower] = i;
        if (lower > 0)
            prev[i] = sub_seq[lower - 1];
    }
    // output
    size = sub_seq.size();
    std::vector<int> result(size);
    seq_index = sub_seq[size - 1];
    for (i = size - 1; i < size; --i)
    {
        result[i] = sequence[seq_index];
        seq_index = prev[seq_index];
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
    SECTION("LongestIncreasingSubsequenceBinarySerach")
    {
        lis_result = LongestIncreasingSubsequenceBinarySerach(sequence);
        lis_solution = { 2, 3, 4, 7, 18 }; 
        REQUIRE(lis_result == lis_solution);
    }
}

TEST_CASE("2")
{
    std::vector<int> sequence, lis_result, lis_solution;
    sequence = { 4, 10, 4, 3, 8, 9 };
    SECTION("LongestIncreasingSubsequenceBinarySerach")
    {
        lis_result = LongestIncreasingSubsequenceBinarySerach(sequence);
        lis_solution = { 3, 8, 9 }; 
        REQUIRE(lis_result == lis_solution);
    }
}

TEST_CASE("3")
{
    std::vector<int> sequence, lis_result, lis_solution;
    sequence = { 0, 1, 0, 3, 2, 3 };
    SECTION("LongestIncreasingSubsequenceBinarySerach")
    {
        lis_result = LongestIncreasingSubsequenceBinarySerach(sequence);
        lis_solution = { 0, 1, 2, 3 }; 
        REQUIRE(lis_result == lis_solution);
    }
}
