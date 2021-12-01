#include <limits.h>
#include <vector>

struct Player
{
    int cost;
    int vorp;
};

struct Result
{
    int total_vorp;
    int total_cost;
    std::vector<int> players;
};

Result SigningPlayers(const std::vector< std::vector<Player> >& players, int budget)
{
    int i, j, c, pos_size, player_size, l_vorp, l_budget;
    pos_size = (int)(players.size());
    player_size = (int)(players[0].size());
    std::vector< std::vector<int> > dp_vorp(pos_size, std::vector<int>(budget + 1)),
        dp_player(pos_size, std::vector<int>(budget + 1));
    std::vector<int> result(pos_size);
    // compute
    for (c = 0; c <= budget; ++c)
    {
        dp_vorp[0][c] = INT_MIN;
        for (j = 0; j < player_size; ++j)
        {
            if (players[0][j].cost <= c && players[0][j].vorp > dp_vorp[0][c])
            {
                dp_vorp[0][c] = players[0][j].vorp;
                dp_player[0][c] = j;
            }
        }
    }
    for (i = 1; i < pos_size; ++i)
    {
        for (c = 0; c <= budget; ++c)
        {
            dp_vorp[i][c] = dp_vorp[i - 1][c];
            dp_player[i][c] = INT_MIN;// means no player for position c
            for (j = 0; j < player_size; ++j)
            {
                if (players[i][j].cost <= c)
                {
                    l_vorp = dp_vorp[i - 1][c - players[i][j].cost] + players[i][j].vorp;
                    if (l_vorp > dp_vorp[i][c])
                    {
                        dp_vorp[i][c] = l_vorp;
                        dp_player[i][c] = j;
                    }
                }
            }
        }
    }
    // output
    l_budget = budget;
    for (i = pos_size - 1; i >= 0; --i)
    {
        result[i] = dp_player[i][l_budget];
        if (result[i] >= 0)
            l_budget -= players[i][result[i]].cost;
    }
    return { dp_vorp[pos_size - 1][budget], // total_vorp
        budget - l_budget, // total_cost
        std::move(result) // players
    };
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("1")
{
    std::cout << "-------------------\n";
    std::vector< std::vector<Player> > players = {
        { { 1000, 100 }, { 2000, 150 } },
        { { 2000, 300 }, { 1000, 200 } },
        { { 1000, 100 }, { 1500, 80 } }
    };
    clock_t start = clock();
    Result result = SigningPlayers(players, 6000);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: \n";
    std::cout << "vorp: " << result.total_vorp << "\n";
    std::cout << "cost: " << result.total_cost << "\n";
    std::cout << "players: ";
    for (int p : result.players)
    {
        std::cout << p << " ";
    }
    std::cout << std::endl;
    std::vector<int> solution_players = { 1, 0, 0 };
    REQUIRE(result.total_vorp == 550);
    REQUIRE(result.total_cost == 5000);
    REQUIRE(result.players == solution_players);
}

TEST_CASE("2")
{
    std::cout << "-------------------\n";
    std::vector< std::vector<Player> > players = {
        { { 1000, 100 }, { 2000, 150 } },
        { { 2000, 300 }, { 1000, 200 } },
        { { 1000, 100 }, { 1500, 80 } }
    };
    clock_t start = clock();
    Result result = SigningPlayers(players, 4000);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: \n";
    std::cout << "vorp: " << result.total_vorp << "\n";
    std::cout << "cost: " << result.total_cost << "\n";
    std::cout << "players: ";
    for (int p : result.players)
    {
        std::cout << p << " ";
    }
    std::cout << std::endl;
    std::vector<int> solution_players = { 0, 0, 0 };
    REQUIRE(result.total_vorp == 500);
    REQUIRE(result.total_cost == 4000);
    REQUIRE(result.players == solution_players);
}

TEST_CASE("3")
{
    std::cout << "-------------------\n";
    std::vector< std::vector<Player> > players = {
        { { 1000, 100 }, { 2000, 150 } },
        { { 2000, 300 }, { 1000, 200 } },
        { { 1000, 90 }, { 1500, 80 } }
    };
    clock_t start = clock();
    Result result = SigningPlayers(players, 3500);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: \n";
    std::cout << "vorp: " << result.total_vorp << "\n";
    std::cout << "cost: " << result.total_cost << "\n";
    std::cout << "players: ";
    for (int p : result.players)
    {
        std::cout << p << " ";
    }
    std::cout << std::endl;
    std::vector<int> solution_players = { 0, 0, INT_MIN };
    REQUIRE(result.total_vorp == 400);
    REQUIRE(result.total_cost == 3000);
    REQUIRE(result.players == solution_players);
}
