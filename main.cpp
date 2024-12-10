#include <iostream>
#include <cstdlib>
#include "./src/game.hpp"
#include "./src/math_stat.hpp"

static constexpr int n_epoch = 100;

int main(int argc, char* argv[]) {
    std::vector<int> matrix;

    int num_args = (argc > 4) ? 4 : argc - 1;

    for (size_t i = 1; i <= num_args; ++i) {
        matrix.emplace_back(std::abs(std::atoi(argv[i])));
    }

    Player a(0.5);
    Player b(0.5);

    Game session(matrix, a, b);
    
    std::vector<int> player_a_points;
    std::vector<int> player_b_points;

    session.both_players_learning(10000, learning_type::both);

    for (size_t i = 0; i < n_epoch; ++i) {
        auto res = session.play_game();
        player_a_points.emplace_back(res.first);
        player_b_points.emplace_back(res.second);
    }

    MathStat calculator(session);

    calculator.do_calc(player_a_points, player_b_points);

    return 0;
}