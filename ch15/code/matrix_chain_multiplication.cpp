#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>

#include <limits.h>
#include <vector>
#include <ostream>
#include <sstream>

typedef std::vector<std::vector<int>> Table;
typedef std::vector<std::vector<int>> Matrix;

inline Table* AllocateTable(int n)
{
    return new Table(n, std::vector<int>(n));
}

inline Matrix* AllocateMatrix(int row, int col)
{
    return new Table(row, std::vector<int>(col));
}

inline int Row(const Table* table)
{
    return table->size();
}

inline int Column(const Table* table)
{
    return table->at(0).size();
}

bool TestResult(const Table* solution, const Table* result)
{
    int i, j;
    for (i = 0; i < Row(solution); ++i)
    {
        for (j = 0; j < Column(solution); ++j)
        {
            if ((*solution)[i][j] >= 0 && (*solution)[i][j] != (*result)[i][j])
                return false;
        }
    }
    return true;
}

/******************************************************************
 * code implement
 ******************************************************************/

/**
 * p: matrix A_i has dimension p[i-1] * p[i]
 *      (i start by 1, so index of p start by 0)
 * return: (m, s)
 * m: table (2d) storing costs (index start by 1)
 * s: table (2d) storing index of k achieved the optimal cost 
 *      (index start by 1)
 * caller is responisble to deallocate return values m and s
 */
std::pair<Table*, Table*> MatrixChainOrder(const std::vector<int>& p)
{
    Table *m, *s;
    int n, i, j, k, l, q;
    n = p.size() - 1;
    m = AllocateTable(n + 1);
    s = AllocateTable(n + 1);
    for (i = 1; i <= n; ++i)
        (*m)[i][i] = 0;
    for (l = 2; l <= n; ++l)
    {
        for (i = 1; i <= n - l + 1; ++i)
        {
            j = i + l - 1;
            (*m)[i][j] = INT_MAX;
            for (k = i; k < j; ++k)
            {
                q = (*m)[i][k] + (*m)[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (q < (*m)[i][j])
                {
                    (*m)[i][j] = q;
                    (*s)[i][j] = k;
                }
            }
        }
    }
    return std::make_pair(m, s);
}

/**
 * s: table (2d) storing index of k achieved the optimal cost 
 *      (index start by 1)
 */
void PrintOptimalParens(std::ostream& os, const Table* s, int i, int j)
{
    if (i == j)
    {
        os << i;
    }
    else
    {
        os << "(";
        PrintOptimalParens(os, s, i, (*s)[i][j]);
        os << ",";
        PrintOptimalParens(os, s, (*s)[i][j] + 1, j);
        os << ")";
    }
}

TEST_CASE("MatrixChainOrder")
{
    std::vector<int> p;
    p = { 30, 35, 15, 5, 10, 20, 25 };
    std::pair<Table*, Table*> result;
    result = MatrixChainOrder(p);
    Table& m = *(result.first);
    Table& s = *(result.second);
    Table solution_m, solution_s;
    solution_m = { 
        { -1, -1, -1, -1, -1, -1, -1 },
        { -1, 0, 15750, 7875, 9375, 11875, 15125 },
        { -1, -1, 0, 2625, 4375, 7125, 10500 },
        { -1, -1, -1, 0, 750, 2500, 5375 },
        { -1, -1, -1, -1, 0, 1000, 3500 },
        { -1, -1, -1, -1, -1, 0, 5000 },
        { -1, -1, -1, -1, -1, -1, 0 },
    };
    solution_s = { 
        { -1, -1, -1, -1, -1, -1, -1 },
        { -1, -1, 1, 1, 3, 3, 3 },
        { -1, -1, -1, 2, 3, 3, 3 },
        { -1, -1, -1, -1, 3, 3, 3 },
        { -1, -1, -1, -1, -1, 4, 5 },
        { -1, -1, -1, -1, -1, -1, 5 },
        { -1, -1, -1, -1, -1, -1, -1 },
    };
    REQUIRE(TestResult(&solution_m, &m));
    REQUIRE(TestResult(&solution_s, &s));
    delete result.first;
    delete result.second;
}

TEST_CASE("PrintOptimalParens")
{
    std::vector<int> p;
    p = { 30, 35, 15, 5, 10, 20, 25 };
    std::pair<Table*, Table*> result;
    result = MatrixChainOrder(p);
    std::stringstream ss;
    PrintOptimalParens(ss, result.second, 1, 6);
    REQUIRE(ss.str() == "((1,(2,3)),((4,5),6))");
    delete result.first;
    delete result.second;
}

/******************************************************************
 * 15.2-1
 ******************************************************************/

TEST_CASE("15.2-1")
{
    std::vector<int> p;
    p = { 5, 10, 3, 12, 5, 50, 6 };
    std::pair<Table*, Table*> result;
    result = MatrixChainOrder(p);
    std::stringstream ss;
    PrintOptimalParens(ss, result.second, 1, 6);
    REQUIRE(ss.str() == "((1,2),((3,4),(5,6)))");
    REQUIRE((*(result.first))[1][6] == (5 * 10 * 3) + (5 * 3 * 6) + 
        (3 * 12 * 5) + (3 * 5 * 6) + (5 * 50 * 6));// 2010
    delete result.first;
    delete result.second;
}

/******************************************************************
 * 15.2-2
 ******************************************************************/

/**
 * caller is responisble to deallocate the return value
 */
Matrix* MatrixMultiply(const Matrix* matrix_a, const Matrix* matrix_b)
{
    int i, j, k;
    Matrix *matrix_c;
    if (Column(matrix_a) != Row(matrix_b))
        throw std::runtime_error("incompatible dimensions");
    matrix_c = AllocateMatrix(Row(matrix_a), Column(matrix_b));
    for (i = 0; i < Row(matrix_a); ++i)
    {
        for (j = 0; j < Column(matrix_b); ++j)
        {
            (*matrix_c)[i][j] = 0;
            for (k = 0; k < Column(matrix_a); ++k)
                (*matrix_c)[i][j] += (*matrix_a)[i][k] * (*matrix_b)[k][j];
        }
    }
    return matrix_c;
}

/**
 * s: table (2d) storing index of k achieved the optimal cost 
 *      (index start by 1)
 * caller is responisble to deallocate the return value
 */
Matrix* MatrixChainMultiply
    (const std::vector<Matrix*>& matrices, const Table* s, int i, int j)
{
    Matrix *matrix_a, *matrix_b, *matrix_c;
    if (i == j)
    {
        return matrices[i - 1];
    }
    matrix_a = MatrixChainMultiply(matrices, s, i, (*s)[i][j]);
    matrix_b = MatrixChainMultiply(matrices, s, (*s)[i][j] + 1, j);
    matrix_c = MatrixMultiply(matrix_a, matrix_b);
    if (i != (*s)[i][j]) delete matrix_a;
    if ((*s)[i][j] + 1 != j) delete matrix_b;
    return matrix_c;
}

TEST_CASE("MatrixChainMultiply")
{
    std::vector<Matrix*> matrices(5);
    std::vector<int> p;
    std::pair<Table*, Table*> result;
    Matrix *matrix_result, *matrix_solution;
    int i;
    p = { 3, 5, 6, 8, 1, 6 };
    result = MatrixChainOrder(p);

    matrices[0] = AllocateMatrix(3, 5);
    matrices[1] = AllocateMatrix(5, 6);
    matrices[2] = AllocateMatrix(6, 3);
    matrices[3] = AllocateMatrix(3, 1);
    matrices[4] = AllocateMatrix(1, 6);
    *(matrices[0]) = {
        { 1, 2, 3, 4, 5 },
        { 6, 7, 8, 9, 10 },
        { 8, 6, 4, 5, 7 }
    };
    *(matrices[1]) = {
        { 2, 8, 2, 3, 5, 5 },
        { 6, 6, 4, 2, 4, 1 },
        { 3, 1, 9, 7, 8, 3 },
        { 7, 5, 6, 5, 7, 3 },
        { 4, 2, 7, 2, 8, 9 }
    };
    *(matrices[2]) = {
        { 4, 5, 2 },
        { 5, 2, 7 },
        { 6, 3, 8 },
        { 7, 6, 4 },
        { 7, 7, 8 },
        { 8, 5, 4 }
    };
    *(matrices[3]) = {
        { 4 },
        { 5 },
        { 9 }
    };
    *(matrices[4]) = {
        { 5, 4, 3, 8, 5, 3 }
    };
    matrix_result = MatrixChainMultiply(matrices, result.second, 1, 5);
    matrix_solution = AllocateMatrix(3, 6);
    (*matrix_solution) = {
        { 230950, 184760, 138570, 369520, 230950, 138570 },
        { 593725, 474980, 356235, 949960, 593725, 356235 },
        { 427860, 342288, 256716, 684576, 427860, 256716 }
    };
    REQUIRE(TestResult(matrix_solution, matrix_result));
    for (i = 0; i < matrices.size(); ++i)
        delete matrices[i]; 
    delete matrix_result;
    delete matrix_solution;
    delete result.first;
    delete result.second;
}
