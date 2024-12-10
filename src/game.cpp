#include "game.hpp"

Game::Game(std::vector<int>& init_matrix, Player& player_a, Player& player_b): points_matrix(std::move(init_matrix)), player_a(player_a), player_b(player_b) {
    std::random_device device;
    random_generator.seed(device());

    probability_matrix = {
        player_a.probability*player_b.probability, player_a.probability*(1-player_b.probability), 
        (1-player_a.probability)*player_b.probability, (1-player_a.probability)*(1-player_b.probability)
    };
}

int Game::simple_generator(double p) {
    std::random_device device;
    random_generator.seed(device());

    std::uniform_real_distribution<double> dist(0, 1);
    return (dist(random_generator) >= p) ? 1: 0;
}

std::pair<int, int> Game::make_move() {
    int player_a_choice = simple_generator(player_a.probability);
    int player_b_choice = simple_generator(player_b.probability);

    switch (player_a_choice) {
        case 0: {
            switch(player_b_choice) {
                case 0: {
                    player_a.points += points_matrix[0];
                    break;
                }
                case 1: {
                    player_b.points += points_matrix[1];
                    break;
                }
            }
            break;
        }
        case 1: {
            switch(player_b_choice) {
                case 0: {
                    player_b.points += points_matrix[2];
                    break;
                }
                case 1: {
                    player_a.points += points_matrix[3];
                    break;
                }
            }
            break;
        }
    }
    return std::make_pair(player_a_choice, player_b_choice);
}

std::pair<int, int> Game::play_game(int n_epoch) {
    reset_players();
    for (size_t i = 0; i < n_epoch; ++i) {
        moves_log.emplace_back(make_move());
    }
    return std::make_pair(player_a.points, player_b.points);
}

void Game::reset_players() {
    player_a.points = 0;
    player_b.points = 0;
}

std::vector<int> Game::get_player_a_weights() {
    std::vector<int> player_a_weights = {points_matrix[0], points_matrix[3]};
    return player_a_weights;
}

std::vector<double> Game::get_player_a_probability() {
    std::vector<double> player_a_probability = {probability_matrix[0], probability_matrix[3]};
    return player_a_probability;
}

std::vector<int> Game::get_player_b_weights() {
    std::vector<int> player_b_weights = {points_matrix[1], points_matrix[2]};
    return player_b_weights;
}

std::vector<double> Game::get_player_b_probability() {
    std::vector<double> player_b_probability = {probability_matrix[1], probability_matrix[2]};
    return player_b_probability;
}

void Game::player_a_learning(int n_epoch, learning_type type) {
    int red_balloons = 1000, blue_balloons = 1000;

    for (size_t i = 0; i < n_epoch; ++i) {
        player_a.probability = static_cast<double>(red_balloons) / (red_balloons + blue_balloons);

        int player_a_choice = simple_generator(player_a.probability);
        int player_b_choice = simple_generator(player_b.probability);

        bool win = (player_a_choice == player_b_choice) ? true: false;

        if (win) {
            if (type == learning_type::reward || type == learning_type::both) {
                if (player_a_choice == 0) red_balloons += points_matrix[0];
                else blue_balloons += points_matrix[3];
            }
        }
        else {
            if (type == learning_type::penalty || type == learning_type::both) {
                if (player_a_choice == 0) red_balloons -= points_matrix[0];
                else blue_balloons -= points_matrix[3];
            }
        }
    }
    update_probability_matrix();
}

void Game::both_players_learning(int n_epoch, learning_type type) {
    int player_a_red_balloons = 1000, player_a_blue_balloons = 1000;
    int player_b_red_balloons = 1000, player_b_blue_balloons = 1000;

    for (size_t i = 0; i < n_epoch; ++i) {
        player_a.probability = static_cast<double>(player_a_red_balloons) / (player_a_red_balloons + player_a_blue_balloons);
        player_b.probability = static_cast<double>(player_b_red_balloons) / (player_b_red_balloons + player_b_blue_balloons);


        int player_a_choice = simple_generator(player_a.probability);
        int player_b_choice = simple_generator(player_b.probability);

        bool win = (player_a_choice == player_b_choice) ? true: false;

        if (win) {
            if (type == learning_type::reward || type == learning_type::both) {
                if (player_a_choice == 0) player_a_red_balloons += points_matrix[0];
                else player_a_blue_balloons += points_matrix[3];
            }

            if (type == learning_type::penalty || type == learning_type::both) {
                if (player_b_choice == 0) player_b_red_balloons -= points_matrix[1];
                else player_b_blue_balloons -= points_matrix[2];
            }
        }
        else {
            if (type == learning_type::penalty || type == learning_type::both) {
                if (player_a_choice == 0) player_a_red_balloons -= points_matrix[0];
                else player_a_blue_balloons -= points_matrix[3];
            }
            
            if (type == learning_type::reward || type == learning_type::both) {
                if (player_b_choice == 0) player_b_red_balloons += points_matrix[1];
                else player_b_blue_balloons += points_matrix[2];
            }
        }
    }
    update_probability_matrix();
}


void Game::update_probability_matrix() {
    probability_matrix = {
        player_a.probability*player_b.probability, player_a.probability*(1-player_b.probability), 
        (1-player_a.probability)*player_b.probability, (1-player_a.probability)*(1-player_b.probability)
    };
}
