#pragma once

struct Player {
    Player(double probability): probability(probability) {}

    int points = 0;

    double probability;

    const bool operator>(Player& other) const 
    {
        return (other.points > this->points);
    }
};