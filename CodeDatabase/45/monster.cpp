#include "monster.hpp"
#include <cstdlib>

Monster::Monster(int health, int damage) 
    : Entity(health, damage) {
}

void Monster::step(Map* map, int x, int y) {
    int player_x = map->getPlayerX();
    int player_y = map->getPlayerY();
    if (abs(x-player_x)+abs(y-player_y) <= 3) {
        if (abs(x-player_x)>=abs(y-player_y)) {
            map->moveEntity(x, y, x-(x-player_x)/abs(x-player_x), y);
        } else {
            map->moveEntity(x, y, x, y-(y-player_y)/abs(y-player_y));
        }
    }
}

std::string Monster::getStatus() const {
    return "Monster," + std::to_string(hp) + "," + std::to_string(damage);
}