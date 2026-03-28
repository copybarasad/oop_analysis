#include "TrapSpell.h"
#include <algorithm>
#include <cmath>

bool TrapSpell::use(GameField& gameField, int x, int y, int current_dist, int& player_boost_level_) {
    if(current_dist > dist) return false;
    bool res = false;
    for(int cy = std::max(0, int(y - std::floor(radius / 2.))); cy < std::min(gameField.getHeight(), int(std::ceil(y + radius / 2.))); cy++) {
        for(int cx = std::max(0, int(x - std::floor(radius / 2.))); cx < std::min(gameField.getWidth(), int(std::ceil(x + radius / 2.))); cx++) {
            if(!gameField.getTrap(cx, cy)) {
                gameField.installTrapAt(cx, cy, value * player_boost_level_);
                res = true;
            }
        }
    }

    if(res) {
        gameField.setColorInArea(
            x,
            y,
            radius,
            sf::Color(78, 53, 242, 255),
            1.0f
        );
    }
    return res;
}

bool TrapSpell::use(GameField& gameField, Tower* tower) {
    return false;
}