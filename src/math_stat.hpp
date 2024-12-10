#pragma once

#include <vector>

class Game;

class MathStat {
private:
    Game& session;
public:
    MathStat(Game& game);

    double mean_value(std::vector<int> points);

    double expected_value(int power, std::vector<int> points, std::vector<double> probability);

    double real_standard_deviation(std::vector<int> points);

    double theoretical_standard_deviation(std::vector<int> weights, std::vector<double> probability, int n_epoch = 100);

    double dispersion(std::vector<int> weights, std::vector<double> probability, int n_epoch = 100);

    void do_calc(std::vector<int> player_a_points, std::vector<int> player_b_points, int n_epoch = 100);
};