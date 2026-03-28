#include "player.h"
#include <iostream>
#include <cmath>

Player::Player(int startX, int startY) 
    : GameEntity(100, 10), position(startX, startY), 
      combatMode(CombatMode::MELEE), meleeDamage(15), rangedDamage(10) {}

std::pair<int, int> Player::getPosition() const {
    return position;
}

void Player::setPosition(int x, int y) {
    position = {x, y};
}

void Player::move(int dx, int dy) {
    position.first += dx;
    position.second += dy;
}

CombatMode Player::getCombatMode() const {
    return combatMode;
}

void Player::switchCombatMode() {
    combatMode = (combatMode == CombatMode::MELEE) ? 
                 CombatMode::RANGED : CombatMode::MELEE;
}

int Player::getMeleeDamage() const {
    return meleeDamage;
}

int Player::getRangedDamage() const {
    return rangedDamage;
}

void Player::increaseMeleeDamage(int amount) {
    meleeDamage += amount;
    if (meleeDamage < 0) meleeDamage = 0;
}

void Player::increaseRangedDamage(int amount) {
    rangedDamage += amount;
    if (rangedDamage < 0) rangedDamage = 0;
}

bool Player::isEnemyInMeleeRange(const std::pair<int, int>& enemyPos) const {
    
    int distance = std::abs(enemyPos.first - position.first) + 
                   std::abs(enemyPos.second - position.second);
    return distance == 1;
}

bool Player::isEnemyInRangedRange(const std::pair<int, int>& enemyPos) const {
    
    int distance = std::abs(enemyPos.first - position.first) + 
                   std::abs(enemyPos.second - position.second);
    return distance <= 3 && distance > 0; 
}