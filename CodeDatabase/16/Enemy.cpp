#include "Enemy.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <sstream>

Enemy::Enemy(int start_x, int start_y, int health, int damage)
    : Entity(start_x, start_y, health, damage) {}

void Enemy::update(Game& game) {
    if (!isAlive()) return;
    
    Player* player = game.getEntityManager().getPlayer();
    if (!player) return;

    Point player_pos = player->getPosition();
    Point enemy_pos = getPosition();

    int move_x = 0, move_y = 0;
    int dx = player_pos.x - enemy_pos.x;
    int dy = player_pos.y - enemy_pos.y;

    if (std::abs(dx) > std::abs(dy)) move_x = (dx > 0) ? 1 : -1;
    else if (dy != 0) move_y = (dy > 0) ? 1 : -1;
    else if (dx != 0) move_x = (dx > 0) ? 1 : -1;

    int new_x = enemy_pos.x + move_x;
    int new_y = enemy_pos.y + move_y;

    if (Point{new_x, new_y} == player_pos) {
        player->takeDamage(getDamage());
        
        std::stringstream ss;
        ss << "An enemy attacks you for " << getDamage() << " damage!";
        game.notifyLog(ss.str());
        
    } else if (game.getField().isValid(new_x, new_y) && game.getField().getCellType(new_x, new_y) != CellType::kImpassable) {
        if (!game.getEntityManager().getEntityAt({new_x, new_y})) {
            setPosition(new_x, new_y);
            game.getEntityManager().checkTrapsFor(game, this);
        }
    }
}