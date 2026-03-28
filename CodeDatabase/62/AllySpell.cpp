#include "AllySpell.h"
#include <algorithm>
#include <cmath>
#include <iostream>

bool AllySpell::use(GameField& gameField, int x, int y, int current_dist, int& player_boost_level_) {
    int spawned_count = 0;
    for(int cy = std::max(0, int(y - std::floor((radius + player_boost_level_ - 1) / 2.))); cy < std::min(gameField.getHeight(), int(std::ceil(y + (radius + player_boost_level_ - 1) / 2.))); cy++) {
        for(int cx = std::max(0, int(x - std::floor((radius + player_boost_level_ - 1) / 2.))); cx < std::min(gameField.getWidth(), int(std::ceil(x + (radius + player_boost_level_ - 1) / 2.))); cx++) {
            std::cout << "Trying to spawn at " << cx << ", " << cy << std::endl;
            if(gameField.isFree(cx, cy) && spawned_count < value + player_boost_level_ - 1) {
                std::cout << "Spawning at " << cx << ", " << cy << std::endl;
                spawned_count++;
                gameField.addAlly(
                    new Enemy("crystalKing", 3.0f, 100, 10),
                    cx,
                    cy
                );
                gameField.setColorInArea(cx, cy, 1, sf::Color(242, 53, 53, 255), 1.0f);
            }
        }
    }
    return spawned_count > 0;
}

bool AllySpell::use(GameField& gameField, Tower* tower) {
    return false;
}