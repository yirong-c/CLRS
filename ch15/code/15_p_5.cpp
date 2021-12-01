#include <limits.h>
#include <string>
#include <vector>
#include <list>

enum Operation { COPY, REPLACE, DELETE, INSERT, TWIDDLE, KILL };

// i and j are index of edit_distance and operations,
// instead of source and target
inline void CheckCopyReplaceInsertDelete(const std::string& source,
    const std::string& target, const std::vector<int>& cost,
    std::vector< std::vector<int> >& edit_distance,
    std::vector< std::vector<Operation> >& operations,
    int i, int j)
{
    int l_cost;
    if (source[i - 1] == target[j - 1])
    {
        l_cost = edit_distance[i - 1][j - 1] + cost[COPY];
        if (l_cost < edit_distance[i][j])
        {
            edit_distance[i][j] = l_cost;
            operations[i][j] = COPY;
        }
    }
    if (source[i - 1] != target[j - 1])
    {
        l_cost = edit_distance[i - 1][j - 1] + cost[REPLACE];
        if (l_cost < edit_distance[i][j])
        {
            edit_distance[i][j] = l_cost;
            operations[i][j] = REPLACE;
        }
    }
    l_cost = edit_distance[i][j - 1] + cost[INSERT];
    if (l_cost < edit_distance[i][j])
    {
        edit_distance[i][j] = l_cost;
        operations[i][j] = INSERT;
    }
    l_cost = edit_distance[i - 1][j] + cost[DELETE];
    if (l_cost < edit_distance[i][j])
    {
        edit_distance[i][j] = l_cost;
        operations[i][j] = DELETE;
    }
}

// O(mn)
std::list<Operation> EditDistance(const std::string& source,
    const std::string& target, const std::vector<int>& cost)
{
    int m, n, i, j, kill_i_pos;
    int l_cost;
    m = (int)(source.size());
    n = (int)(target.size());
    std::list<Operation> result;
    /**
     * edit_distance[i + 1][j + 1] is the edit distance
     * from source[0 ... i] to target[0 ... j]
     */
    std::vector< std::vector<int> > edit_distance(m + 1, 
        std::vector<int>(n + 1));
    /**
     * operations[i + 1][j + 1] is the operation
     * which incremented offset of source and target to i and j
     */
    std::vector< std::vector<Operation> > operations(m + 1, 
        std::vector<Operation>(n + 1));
    // compute
    edit_distance[0][0] = 0;
    for (i = 1; i <= m; ++i)// j = 0
    {
        edit_distance[i][0] = edit_distance[i - 1][0] + cost[DELETE];
        operations[i][0] = DELETE;
    }
    for (j = 1; j <= n; ++j)// i = 0
    {
        edit_distance[0][j] = edit_distance[0][j - 1] + cost[INSERT];
        operations[0][j] = INSERT;
    }
    for (i = 1; i <= m; ++i)// j = 1
    {
        edit_distance[i][1] = INT_MAX;
        CheckCopyReplaceInsertDelete(source, target, cost, 
            edit_distance, operations, i, 1);
    }
    for (j = 2; j <= n; ++j)// i = 1
    {
        edit_distance[1][j] = INT_MAX;
        CheckCopyReplaceInsertDelete(source, target, cost, 
            edit_distance, operations, 1, j);
    }
    for (i = 2; i <= m; ++i)
    {
        for (j = 2; j <= n; ++j)
        {
            edit_distance[i][j] = INT_MAX;
            CheckCopyReplaceInsertDelete(source, target, cost, 
                edit_distance, operations, i, j);
            if (source[i - 2] == target[j - 1] && 
                source[i - 1] == target[j - 2])
            {
                l_cost = edit_distance[i - 2][j - 2] + cost[TWIDDLE];
                if (l_cost < edit_distance[i][j])
                {
                    edit_distance[i][j] = l_cost;
                    operations[i][j] = TWIDDLE;
                }
            }
        }
    }
    for (i = 0; i < m; ++i)
    {
        l_cost = edit_distance[i][n] + cost[KILL];
        if ( l_cost < edit_distance[m][n])
        {
            edit_distance[m][n] = l_cost;
            operations[m][n] = KILL;
            kill_i_pos = i;
        }
    }
    // output
    i = m;
    j = n;
    while (i != 0 || j != 0)
    {
        result.push_front(operations[i][j]);
        switch (operations[i][j])
        {
        case COPY:
            --i;
            --j;
            break;
        case REPLACE:
            --i;
            --j;
            break;
        case DELETE:
            --i;
            break;
        case INSERT:
            --j;
            break;
        case TWIDDLE:
            i -= 2;
            j -= 2;
            break;
        case KILL:
            i = kill_i_pos;
            break;
        }
    }
    return result;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("example")
{
    std::cout << "------------------------" << std::endl;
    std::string source = "algorithm";
    std::string target = "altruistic";
    // COPY, REPLACE, DELETE, INSERT, TWIDDLE, KILL
    std::vector<int> cost = { 18, 23, 50, 50, 5, 2 };
    clock_t start = clock();
    std::list<Operation> result = EditDistance(source, target, cost);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: ";
    for (Operation i : result)
    {
        switch (i)
        {
        case COPY:
            std::cout << "COPY ";
            break;
        case REPLACE:
            std::cout << "REPLACE ";
            break;
        case DELETE:
            std::cout << "DELETE ";
            break;
        case INSERT:
            std::cout << "INSERT ";
            break;
        case TWIDDLE:
            std::cout << "TWIDDLE ";
            break;
        case KILL:
            std::cout << "KILL ";
            break;
        }
    }
    std::list<Operation> solution = 
        { COPY, COPY, INSERT, INSERT, REPLACE, REPLACE,
            REPLACE, TWIDDLE, REPLACE, KILL };
    std::cout << std::endl;
    REQUIRE(result == solution);
}
