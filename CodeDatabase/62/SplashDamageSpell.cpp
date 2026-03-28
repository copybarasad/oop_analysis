#include "SplashDamageSpell.h"
#include <algorithm>
#include <cmath>

bool SplashDamageSpell::use(GameField& gameField, int x, int y, int current_dist, int& player_boost_level_) {
    if(current_dist > dist) return false;
    std::vector<Enemy*> enemies = gameField.getEnemiesInArea(
        x,
        y,
        radius + player_boost_level_ - 1
    );

    std::vector<Tower*> towers = gameField.getTowersInArea(
        x,
        y,
        radius + player_boost_level_ - 1
    );

    gameField.setColorInArea(
        x,
        y,
        radius + player_boost_level_ - 1,
        sf::Color(179, 30, 16, 255),
        1.0f
    );

    for(Enemy* enemy : enemies) {
        std::pair<int, int> pos = gameField.getEnemyCell(enemy);
        gameField.applyDamage(value * player_boost_level_, pos.first, pos.second, 0.1f);
    }

    for(Tower* tower : towers) {
        std::pair<int, int> pos = gameField.getTowerCell(tower);
        gameField.applyDamage(value * player_boost_level_, pos.first, pos.second, 0.1f);
    }

    return true;
}

bool SplashDamageSpell::use(GameField& gameField, Tower* tower) {
    std::pair<int, int> pos = gameField.getTowerCell(tower);
    int r = tower->getRadius();
    int cx = pos.first, cy = pos.second;
    std::pair<int, int> p = gameField.getPlayerCell();
    for(int y = std::max(0, int(cy - std::floor(r / 2.))); y < std::min(gameField.getHeight(), int(std::ceil(cy + r / 2.))); y++) {
        for(int x = std::max(0, int(cx - std::floor(r / 2.))); x < std::min(gameField.getWidth(), int(std::ceil(cx + r / 2.))); x++)  {
            if((x == p.first && y == p.second) || (gameField.getAllyAt(x, y) != nullptr)) {
                gameField.applyDamage(value * 0.6, x, y, 0.1f);
            }
        }
    }
    return true;
}