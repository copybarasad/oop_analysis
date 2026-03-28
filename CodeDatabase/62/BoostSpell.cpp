#include "BoostSpell.h"

bool BoostSpell::use(GameField& gameField, int x, int y, int current_dist, int& player_boost_level_) {
    std::pair<int, int> pos = gameField.getPlayerCell();
    if(pos.first < 0 || pos.second < 0 || current_dist > dist) return false;
    gameField.setColorInArea(x, y, radius, sf::Color::Green, 0.5f);
    gameField.setColorInArea(pos.first, pos.second, radius, sf::Color::Green, 0.5f);
    player_boost_level_ += value;
    return true;
}

bool BoostSpell::use(GameField& gameField, Tower* tower) {
    return false;
}