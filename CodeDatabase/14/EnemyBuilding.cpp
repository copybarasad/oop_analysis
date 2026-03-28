#include "EnemyBuilding.hpp"
#include "player.hpp"
#include "GameBoard.hpp"

EnemyBuilding::EnemyBuilding(int x, int y, int spawn_rate)
    : Entity(x, y), 
      health(ENEMY_BUILDING_HEALTH),
      turns_to_spawn(spawn_rate), 
      initial_turns_to_spawn(spawn_rate) {}

bool EnemyBuilding::shouldSpawn() {
    if (turns_to_spawn > 0) {
        turns_to_spawn--;
    }
    return turns_to_spawn == 0;
}

void EnemyBuilding::resetSpawnTimer() {
    turns_to_spawn = initial_turns_to_spawn;
}

void EnemyBuilding::takeDamage(int damage) {
    health -= damage;
}

int EnemyBuilding::getHealth() const {
    return health;
}

bool EnemyBuilding::isAlive() const {
    return health > 0;
}

void EnemyBuilding::onDeath(Player* player) {
    if (player) {
        player->addScore(SCORE_PER_BUILDING_DESTROYED);
        player->incrementKillCount();
    }
}

bool EnemyBuilding::isEnemy() const {
    return true;
}

void EnemyBuilding::removeFromBoard(GameBoard& board) {
    board.removeBuilding(this);
}
