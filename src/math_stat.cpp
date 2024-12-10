#include "math_stat.hpp"
#include "game.hpp"

#include <math.h>
#include <iomanip>

MathStat::MathStat(Game& game): session(game) {}

double MathStat::real_standard_deviation(std::vector<int> points) {
    double mean_val = MathStat::mean_value(points);

    double res = 0;

    for (size_t i = 0; i < points.size(); ++i) {
        res += pow((double(points[i]) - mean_val), 2);
    }

    res /= (points.size());

    return sqrt(res);
}

double MathStat::theoretical_standard_deviation(std::vector<int> weights, std::vector<double> probability, int n_epoch) {
    auto d = dispersion(weights, probability, n_epoch);

    return sqrt(d);
}

double MathStat::expected_value(int power, std::vector<int> weights, std::vector<double> probability) {
    double player_expectation = 0;

    for (int i = 0; i < weights.size(); ++i) {
        player_expectation += (pow(weights[i], power) * probability[i]);
    } 

    return player_expectation;
}

double MathStat::mean_value(std::vector<int> points) {
    double sum = 0;
    for (const auto point: points) {
        sum += point;
    }
    return sum / points.size();
}

double MathStat::dispersion(std::vector<int> weights, std::vector<double> probability, int n_epoch) { 
    auto expectation_squared = expected_value(2, weights, probability) * n_epoch;
    auto squared_expectation = pow(expected_value(1, weights, probability), 2) * n_epoch;

    return expectation_squared - squared_expectation;
}

void MathStat::do_calc(std::vector<int> player_a_points, std::vector<int> player_b_points, int n_epoch) {

    double mean_player_a = mean_value(player_a_points);
    double mean_player_b = mean_value(player_b_points);

    double expectation_player_a = expected_value(1, session.get_player_a_weights(), session.get_player_a_probability()) * n_epoch;
    double expectation_player_b = expected_value(1, session.get_player_b_weights(), session.get_player_b_probability()) * n_epoch;

    double real_std_player_a = real_standard_deviation(player_a_points);
    double real_std_player_b = real_standard_deviation(player_b_points);

    double theoretical_std_player_a = theoretical_standard_deviation(session.get_player_a_weights(), session.get_player_a_probability());
    double theoretical_std_player_b = theoretical_standard_deviation(session.get_player_b_weights(), session.get_player_b_probability());

    double dispers_player_a = dispersion(session.get_player_a_weights(), session.get_player_a_probability());
    double dispers_player_b = dispersion(session.get_player_b_weights(), session.get_player_b_probability());

    std::cout << std::setw(35) << std::left << "mean value: " << std::setw(15) << std::right << mean_player_a << std::setw(15) << std::right << mean_player_b << std::endl;
    std::cout << std::setw(35) << std::left << "math expectation: " << std::setw(15) << std::right << expectation_player_a << std::setw(15) << std::right << expectation_player_b << std::endl;
    std::cout << std::setw(35) << std::left << "experimental standard deviation: " << std::setw(15) << std::right << real_std_player_a << std::setw(15) << std::right << real_std_player_b << std::endl;
    std::cout << std::setw(35) << std::left << "theoretical standard deviation: " << std::setw(15) << std::right << theoretical_std_player_a << std::setw(15) << std::right << theoretical_std_player_b << std::endl;
    std::cout << std::setw(35) << std::left << "theoretical dispersion: " << std::setw(15) << std::right << dispers_player_a << std::setw(15) << std::right << dispers_player_b << std::endl;
}