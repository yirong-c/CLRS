#include <vector>

#define INF 2000000000 // 2 billion

// return a 4-elements array in [pennies, nickels, dimes, quarters]
std::vector<int> CoinChanging(int cents) {
    int pennies, nickels, dimes, quarters;
    quarters = cents / 25;
    cents = cents % 25;
    dimes = cents / 10;
    cents = cents % 10;
    nickels = cents / 5;
    pennies = cents % 5;
    return { pennies, nickels, dimes, quarters };
}

// assume penny is included in the parameter denominations
// return an array contains the numbers of coins 
// corresponding by the elements in the parameter denominations
std::vector<int> CoinChanging(const std::vector<int>& denominations, int cents) {
    int k = (int)(denominations.size());
    std::vector<int> dp_num(cents + 1), dp_coin(cents + 1);
    // compute
    dp_num[0] = 0;
    for (int i = 1; i <= cents; ++i) {
        dp_num[i] = INF;
        for (int j = 0; j < k; ++j) {
            if (i - denominations[j] >= 0 && 
                    dp_num[i - denominations[j]] < dp_num[i]) {
                dp_num[i] = dp_num[i - denominations[j]];
                dp_coin[i] = j;
            }
        }
        ++dp_num[i];
    }
    // output
    std::vector<int> result(k, 0);
    for (int i = cents; i > 0; i -= denominations[dp_coin[i]]) {
        ++result[dp_coin[i]];
    }
    return result;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("(a)") {
    REQUIRE(CoinChanging(0) == std::vector<int>{ 0, 0, 0, 0 });
    REQUIRE(CoinChanging(88) == std::vector<int>{ 3, 0, 1, 3 });
    REQUIRE(CoinChanging(93) == std::vector<int>{ 3, 1, 1, 3 });
    REQUIRE(CoinChanging(100) == std::vector<int>{ 0, 0, 0, 4 });
}

TEST_CASE("(d)") {
    REQUIRE(CoinChanging({ 1, 5, 10, 25 }, 0) == std::vector<int>{ 0, 0, 0, 0 });
    REQUIRE(CoinChanging({ 1, 5, 10, 25 }, 88) == std::vector<int>{ 3, 0, 1, 3 });
    REQUIRE(CoinChanging({ 1, 5, 10, 25 }, 93) == std::vector<int>{ 3, 1, 1, 3 });
    REQUIRE(CoinChanging({ 1, 5, 10, 25 }, 100) == std::vector<int>{ 0, 0, 0, 4 });
    REQUIRE(CoinChanging({ 1, 3, 4, 5 }, 7) == std::vector<int>{ 0, 1, 1, 0 });
    REQUIRE(CoinChanging({ 1, 6, 14 }, 18) == std::vector<int>{ 0, 3, 0 });
    REQUIRE(CoinChanging({ 1, 10, 25 }, 30) == std::vector<int>{ 0, 3, 0 });
}
