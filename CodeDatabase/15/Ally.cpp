#include "Ally.h"
#include "Game.h"
#include "Enemy.h"
#include <cmath>
#include <iostream>
#include <sstream>

Ally::Ally(int start_x, int start_y, int health, int damage)
    : Entity(start_x, start_y, health, damage) {}

void Ally::update(Game& game) {
    if (!isAlive()) return;
    
    auto& enemies = game.getEntityManager().getEnemies();
    if (enemies.empty()) return;

    Enemy* closest_enemy = nullptr;
    double min_dist = -1;

    for (const auto& enemy : enemies) {
        int dx = enemy->getPosition().x - getPosition().x;
        int dy = enemy->getPosition().y - getPosition().y;
        double dist = std::sqrt(dx * dx + dy * dy);
        if (!closest_enemy || dist < min_dist) {
            min_dist = dist;
            closest_enemy = enemy.get();
        }
    }
    
    if (!closest_enemy) return;

    Point ally_pos = getPosition();
    Point enemy_pos = closest_enemy->getPosition();
    int dx = enemy_pos.x - ally_pos.x;
    int dy = enemy_pos.y - ally_pos.y;

    if (std::abs(dx) <= 1 && std::abs(dy) <= 1) {
         std::stringstream ss;
         ss << "An ally attacks an enemy for " << getDamage() << " damage!";
         game.notifyLog(ss.str());
         
         if (closest_enemy->takeDamage(getDamage())) {
             game.notifyLog("An ally defeated an enemy!");
             if(auto player = game.getEntityManager().getPlayer()) player->addScore(10);
         }
    } else {
        int move_x = 0, move_y = 0;
        if (std::abs(dx) > std::abs(dy)) move_x = (dx > 0) ? 1 : -1;
        else if (dy != 0) move_y = (dy > 0) ? 1 : -1;
        else if (dx != 0) move_x = (dx > 0) ? 1 : -1;

        int new_x = ally_pos.x + move_x;
        int new_y = ally_pos.y + move_y;

        Point new_pos = {new_x, new_y};
        if (game.getField().isValid(new_x, new_y) && game.getField().getCellType(new_x, new_y) != CellType::kImpassable && !game.getEntityManager().getEntityAt(new_pos)) {
            setPosition(new_x, new_y);
        }
    }
}