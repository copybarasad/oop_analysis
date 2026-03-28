#include "Enemy.hpp"
#include "Logger.hpp"

Enemy::Enemy(int hp, int damage, Position pos, Logger& logger)
    : Character(hp, damage, pos, logger) {}

bool Enemy::chase(Grid& grid, Player& player) {
    Position player_pos = player.getPos();
    Position enemy_pos = pos_;
    int dx = (player_pos.x > enemy_pos.x) ? 1 : (player_pos.x < enemy_pos.x ? -1 : 0);
    int dy = (player_pos.y > enemy_pos.y) ? 1 : (player_pos.y < enemy_pos.y ? -1 : 0);
    Position nextPos{enemy_pos.x + dx, enemy_pos.y + dy};

    if(!grid.IsFreeCell(nextPos.x, nextPos.y)) return false;
    if(grid.slowedCell(nextPos.x, nextPos.y)) {
        logger_.warning("Enemy stepped on slowed tile - movement blocked!");
        return false;
    }
    int dist = distanceTo(player);
    if(dist <=1 ) {
        logger_.action("Enemy attacks player!");
        player.takeDamage(damage_);
        return true;
    }

    setPos(nextPos);
    logger_.action("Enemy moved to (" + std::to_string(pos_.x) + "," + std::to_string(pos_.y) + ")");
    return true;
}
