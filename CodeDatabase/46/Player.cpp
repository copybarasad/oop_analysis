#include "Player.h"

Player::Player(const std::string& playerName, int startX, int startY) 
    : name(playerName), health(100), maxHealth(100), damage(15), score(0), 
      positionX(startX), positionY(startY), combatMode(CombatMode::MELEE), attackRange(1) {}

std::string Player::getName() const { return name; }
int Player::getHealth() const { return health; }
int Player::getMaxHealth() const { return maxHealth; }
int Player::getDamage() const { return damage; }
int Player::getScore() const { return score; }
int Player::getPositionX() const { return positionX; }
int Player::getPositionY() const { return positionY; }
CombatMode Player::getCombatMode() const { return combatMode; }
int Player::getAttackRange() const { return attackRange; }

void Player::setPosition(int x, int y) {
    positionX = x;
    positionY = y;
}

void Player::takeDamage(int damageAmount) {
    health -= damageAmount;
    if (health < 0) health = 0;
}

void Player::heal(int amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

void Player::addScore(int points) {
    score += points;
}

void Player::switchCombatMode() {
    combatMode = (combatMode == CombatMode::MELEE) ? CombatMode::RANGED : CombatMode::MELEE;
    
    if (combatMode == CombatMode::MELEE) {
        damage = 15;
        attackRange = 1;
    } else {
        damage = 8;
        attackRange = 3;
    }
}

bool Player::isAlive() const {
    return health > 0;
}

bool Player::move(int deltaX, int deltaY) {
    positionX += deltaX;
    positionY += deltaY;
    return true;
}