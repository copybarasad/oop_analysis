#include "MainEnemy.hpp"

MainEnemy::MainEnemy(int hp, int damage, Position pos, Logger& logger)
    : Character(hp, damage, pos, logger) {}

std::unique_ptr<EnemyConstruction> MainEnemy::putConstruction(Position pos, Grid& grid, Logger& logger) {
    if(!grid.IsFreeCell(pos.x,pos.y)) {
        logger_.warning("Cannot place construction here!");
        return nullptr;
    }
    grid.setObj(pos.x,pos.y,cellType::forbidden);
    logger_.action("Main enemy placed construction at ("+std::to_string(pos.x)+","+std::to_string(pos.y)+")");
    return std::make_unique<EnemyConstruction>(5, pos, grid, logger);
}


bool MainEnemy::chase(Grid& grid, Player& player) {
    Position player_pos = player.getPos();
    Position enemy_pos = pos_;
    int dx = (player_pos.x > enemy_pos.x) ? 1 : (player_pos.x < enemy_pos.x ? -1 : 0);
    int dy = (player_pos.y > enemy_pos.y) ? 1 : (player_pos.y < enemy_pos.y ? -1 : 0);
    Position nextPos{enemy_pos.x + dx, enemy_pos.y + dy};

    if(!grid.IsFreeCell(nextPos.x, nextPos.y)) return false;
    if(grid.slowedCell(nextPos.x, nextPos.y)) {
        logger_.warning("Main enemy stepped on slowed tile - movement blocked!");
        return false;
    }
    int dist = distanceTo(player);
    if(dist <=1 ) {
        logger_.action("Enemy attacks player at" + std::to_string(nextPos.x) + "," + std::to_string(nextPos.y));
        player.takeDamage(damage_);
        return true;
    }

    setPos(nextPos);
    logger_.action("Enemy moved to (" + std::to_string(pos_.x) + "," + std::to_string(pos_.y) + ")");
    return true;
}
