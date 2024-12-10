#pragma once

#include <iostream>
#include <vector>
#include <random>

#include "player.hpp"

enum class learning_type {reward, penalty, both};

class Game {
private:
    friend class MathStat;
    std::vector<int> points_matrix;

    Player& player_a;
    Player& player_b;

    std::mt19937 random_generator;

    std::vector<std::pair<int, int>> moves_log;

    std::vector<double> probability_matrix;
public:
    Game(std::vector<int>& init_matrix, Player& player_a, Player& player_b);

    int simple_generator(double p = 0.5);

    std::pair<int, int> make_move();

    void reset_players();

    std::pair<int, int> play_game(int n_epoch = 100);

    std::vector<int> get_player_a_weights();

    std::vector<double> get_player_a_probability();

    std::vector<int> get_player_b_weights();

    std::vector<double> get_player_b_probability();

    void player_a_learning(int n_epoch = 1000, learning_type type = learning_type::reward);

    void both_players_learning(int n_epoch = 1000, learning_type type = learning_type::both);

    void update_probability_matrix();
};