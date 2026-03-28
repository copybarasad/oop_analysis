#include <iostream>
#include "towerspell.h"
bool TowerDirectSpell::do_spell(int x, int y, Field& field, Player& player, Enemy& enemy, std::string& message) {
    return false;
}
bool TowerDirectSpell::do_spell(int targetX, int targetY, Player& target, std::string& message) {
    if (targetX == target.getX() && targetY == target.getY() && target.isAlive()) {
        target.takeDamage(damage);
        message = name + " наносит " + std::to_string(damage) + " урона игроку!";
        return true;
    }
    return false;
}
    
std::string TowerDirectSpell::getName() const { return name; }
int TowerDirectSpell::getCost() const { return 0; }
int TowerDirectSpell::getRange() const { return range; }
