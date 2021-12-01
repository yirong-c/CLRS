#include <vector>
#include <limits.h>

using Row = std::vector<int>;
using Table = std::vector<Row>;

// O(n^2)
std::pair<Table, Table> OptimalBST(const Row& p, const Row& q)
{
    size_t q_size, i, j, l, r;
    int t;
    q_size = q.size();// n + 1
    Table e(q_size + 1, Row(q_size)), w(q_size + 1, Row(q_size)),
        root(q_size, Row(q_size));
    for (i = 1; i <= q_size; ++i)// 1 to (n + 1)
    {
        e[i][i - 1] = q[i - 1];
        // w[i][i - 1] = q[i - 1];
    }
    // l = 1
    for (i = 1; i < q_size; ++i)// 1 to n
    {
        // w[i][i] = w[i][i - 1] + p[i] + q[i];
        w[i][i] = q[i - 1] + p[i] + q[i];
        // e[i][i] = e[i][i - 1] + e[i + 1][i] + w[i][i];
        e[i][i] = q[i - 1] + q[i] + w[i][i];
        root[i][i] = i;
    }
    // l = 2 ... n
    for (l = 2; l < q_size; ++l)// 2 to n
    {
        for (i = 1; i <= q_size - l; ++i)// 1 to (n - l + 1)
        {
            j = i + l - 1;
            e[i][j] = INT_MAX;
            // root[i][j - 1] to root[i + 1][j]
            for (r = root[i][j - 1]; r <= root[i + 1][j]; ++r)
            {
                t = e[i][r - 1] + e[r + 1][j];
                if (t < e[i][j])
                {
                    e[i][j] = t;
                    root[i][j] = r;
                }
            }
            w[i][j] = w[i][j - 1] + p[j] + q[j];
            e[i][j] += w[i][j];
        }
    }
    return std::make_pair(std::move(e), std::move(root));
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
template <typename T>

bool CheckTable(const T& a, const T& b)
{
    size_t i, j;
    assert(a.size() == b.size());
    auto b_row_it = b.cbegin();
    i = 0;
    for (const auto& a_row : a)
    {
        assert(a_row.size() == b_row_it->size());
        auto b_col_it = b_row_it->cbegin();
        j = 0;
        for (const auto& a_col : a_row)
        {
            if (a_col >= 0 && *b_col_it >= 0)
                assert(a_col == *b_col_it);
            ++b_col_it;
            ++j;
        }
        ++b_row_it;
        ++i;
    }
    return true;
}

TEST_CASE("example")
{
    Row p = { INT_MIN, 15, 10, 5, 10, 20 };
    Row q = { 5, 10, 5, 5, 5, 10 };
    std::pair<Table, Table> result = OptimalBST(p, q);
    Table& e_result = result.first;
    Table& root_result = result.second;
    Table e_solution = {
        { INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN },
        { 5, 45, 90, 125, 175, 275 },
        { INT_MIN, 10, 40, 70, 120, 200 },
        { INT_MIN, INT_MIN, 5, 25, 60, 130 },
        { INT_MIN, INT_MIN, INT_MIN, 5, 30, 90 },
        { INT_MIN, INT_MIN, INT_MIN, INT_MIN, 5, 50 },
        { INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, 10 }
    };
    Table root_solution = {
        { INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN },
        { INT_MIN, 1, 1, 2, 2, 2 },
        { INT_MIN, INT_MIN, 2, 2, 2, 4 },
        { INT_MIN, INT_MIN, INT_MIN, 3, 4, 5 },
        { INT_MIN, INT_MIN, INT_MIN, INT_MIN, 4, 5 },
        { INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, 5 }
    };
    REQUIRE(CheckTable(e_result, e_solution));
    REQUIRE(CheckTable(root_result, root_solution));
}
