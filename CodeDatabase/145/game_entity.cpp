#include "game_entity.h"
#include "exceptions.h"

GameEntity::GameEntity(int health, int damage) 
    : health(health), maxHealth(health), damage(damage), score(0) {
    if (health <= 0) {
        throw GameLogicException("GameEntity: health must be positive, got " + std::to_string(health));
    }
    if (damage < 0) {
        throw GameLogicException("GameEntity: damage cannot be negative, got " + std::to_string(damage));
    }
}

int GameEntity::getHealth() const {
    return health;
}

void GameEntity::setHealth(int h) {
    health = h;
    if (health < 0) health = 0;
    if (health > maxHealth) health = maxHealth;
}

int GameEntity::getMaxHealth() const {
    return maxHealth;
}

void GameEntity::setMaxHealth(int h) {
    maxHealth = h;
    if (maxHealth < 1) maxHealth = 1;
    if (health > maxHealth) health = maxHealth;
}

int GameEntity::getDamage() const {
    return damage;
}

int GameEntity::getScore() const {
    return score;
}

void GameEntity::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

void GameEntity::increaseScore(int points) {
    score += points;
    if (score < 0) score = 0;
}

bool GameEntity::isAlive() const {
    return health > 0;
}