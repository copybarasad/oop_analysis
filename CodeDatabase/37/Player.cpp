#include "Player.h"

Player::Player(int startHealth, int startMeleeDamage, int startRangedDamage)
    : health(startHealth), meleeDamage(startMeleeDamage), 
      rangedDamage(startRangedDamage), isRangedMode(false), 
      position(0, 0), canMove(true) {}

void Player::move(const Position& newPos) {
    if (canMove) {
        position = newPos;
    }
}

void Player::switchCombatMode() {
    isRangedMode = !isRangedMode;
    
}

void Player::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}