#include <vector>
#include <limits.h>
#include <string>
#include <sstream>

using Row = std::vector<int>;
using Table = std::vector<Row>;

// O(n^3)
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
        w[i][i - 1] = q[i - 1];
    }
    for (l = 1; l < q_size; ++l)// 1 to n
    {
        for (i = 1; i <= q_size - l; ++i)// 1 to (n - l + 1)
        {
            j = i + l - 1;
            e[i][j] = INT_MAX;
            for (r = i; r <= j; ++r)// i to j
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

void ConstructOptimalBST(std::ostream& os, const Table& root, 
    const std::string& suffix, size_t i, size_t j)
{
    if (j == i - 1)
    {
        os << "d_" << j << " is the " << suffix << "\n";
    }
    else
    {
        size_t r = root[i][j];
        os << "k_" << r << " is the " << suffix << "\n";
        ConstructOptimalBST(os, root, 
            "left child of k_" + std::to_string(r), i, r - 1);
        ConstructOptimalBST(os, root, 
            "right child of k_" + std::to_string(r), r + 1, j);
    }
}

void ConstructOptimalBST(std::ostream& os, const Table& root)
{
    ConstructOptimalBST(os, root, "root", 1, root.size() - 1);
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
    std::stringstream construct_result_ss;
    ConstructOptimalBST(construct_result_ss, root_result);
    std::string construct_solution = 
        "k_2 is the root\n"
        "k_1 is the left child of k_2\n"
        "d_0 is the left child of k_1\n"
        "d_1 is the right child of k_1\n"
        "k_5 is the right child of k_2\n"
        "k_4 is the left child of k_5\n"
        "k_3 is the left child of k_4\n"
        "d_2 is the left child of k_3\n"
        "d_3 is the right child of k_3\n"
        "d_4 is the right child of k_4\n"
        "d_5 is the right child of k_5\n";
    REQUIRE(construct_result_ss.str() == construct_solution);
}

TEST_CASE("15.5-2")
{
    Row p = { INT_MIN, 4, 6, 8, 2, 10, 12, 14 };
    Row q = { 6, 6, 6, 6, 5, 5, 5, 5 };
    std::pair<Table, Table> result = OptimalBST(p, q);
    Table& e_result = result.first;
    Table& root_result = result.second;
    REQUIRE(e_result[1][7] == 312);
    std::stringstream construct_result_ss;
    ConstructOptimalBST(construct_result_ss, root_result);
    std::string construct_solution = 
        "k_5 is the root\n"
        "k_2 is the left child of k_5\n"
        "k_1 is the left child of k_2\n"
        "d_0 is the left child of k_1\n"
        "d_1 is the right child of k_1\n"
        "k_3 is the right child of k_2\n"
        "d_2 is the left child of k_3\n"
        "k_4 is the right child of k_3\n"
        "d_3 is the left child of k_4\n"
        "d_4 is the right child of k_4\n"
        "k_7 is the right child of k_5\n"
        "k_6 is the left child of k_7\n"
        "d_5 is the left child of k_6\n"
        "d_6 is the right child of k_6\n"
        "d_7 is the right child of k_7\n";
    REQUIRE(construct_result_ss.str() == construct_solution);
}
