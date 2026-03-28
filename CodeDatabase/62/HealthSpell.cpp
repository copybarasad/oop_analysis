#include "HealthSpell.h"

bool HealthSpell::use(GameField& gameField, int x, int y, int current_dist, int& player_boost_level_) {
    if(current_dist > dist + player_boost_level_ - 1) return false;
    std::vector<Entity*> allies = gameField.getAlliesInAreaWithPlayer(
        x, 
        y, 
        radius
    );
    gameField.setColorInArea(
        x,
        y,
        radius,
        sf::Color(82, 242, 53, 255),
        1.0f
    );

    for(Entity* a: allies) {
        a->applyHeal(value);
    }

    return true;
}

bool HealthSpell::use(GameField& gameField, Tower* tower) {
    return false;
}