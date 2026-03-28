#include "EnemyTower.hpp"
#include "player.hpp"
#include "GameBoard.hpp"

EnemyTower::EnemyTower(int x, int y)
    : Entity(x, y),
      health(ENEMY_TOWER_HEALTH),
      damage(ENEMY_TOWER_DAMAGE),
      range(ENEMY_TOWER_RANGE),
      cooldown(ENEMY_TOWER_COOLDOWN),
      current_cooldown(0) {}

void EnemyTower::takeDamage(int dmg) {
    health -= dmg;
}

bool EnemyTower::isAlive() const {
    return health > 0;
}

int EnemyTower::getHealth() const {
    return health;
}

int EnemyTower::getDamage() const {
    return damage;
}

double EnemyTower::getRange() const {
    return range;
}

bool EnemyTower::canAttack() const {
    return current_cooldown <= 0;
}

void EnemyTower::tickCooldown() {
    if (current_cooldown > 0) {
        current_cooldown--;
    }
}

void EnemyTower::resetCooldown() {
    current_cooldown = cooldown;
}

void EnemyTower::onDeath(Player* player) {
    if (player) {
        player->addScore(SCORE_PER_TOWER_DESTROYED);
        player->incrementKillCount();
    }
}

bool EnemyTower::isEnemy() const {
    return true;
}

void EnemyTower::removeFromBoard(GameBoard& board) {
    board.removeTower(this);
}

