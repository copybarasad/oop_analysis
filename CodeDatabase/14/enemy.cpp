#include "enemy.hpp"
#include "player.hpp"
#include "GameBoard.hpp"

Enemy::Enemy(int x, int y, int start_health, int enemy_damage)
    : Entity(x, y), health(start_health), damage(enemy_damage) {}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getDamage() const {
    return damage;
}

void Enemy::takeDamage(int dmg) {
    health -= dmg;
}

bool Enemy::isAlive() const {
    return health > 0;
}

bool Enemy::isEnemy() const {
    return true;
}

void Enemy::onDeath(Player* player) {
    if (player) {
        player->addScore(SCORE_PER_KILL);
        player->incrementKillCount();
    }
}

void Enemy::removeFromBoard(GameBoard& board) {
    board.removeEnemy(this);
}
