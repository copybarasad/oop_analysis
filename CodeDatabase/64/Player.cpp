#include "Player.h"
#include "Enemy.h"
#include "GameField.h"
#include "Spell.h"
#include <iostream>
#include <cmath>

Player::Player(int startX, int startY) 
    : Character(100, 10, startX, startY), totalScore(0), 
      currentCombatStyle(MELEE_STYLE), spellHand(3) {
    attackDamage = 15;
}

void Player::addToScore(int points) {
    totalScore += points;
    std::cout << "+" << points << " points! Total: " << totalScore << std::endl;
}

int Player::getTotalScore() const { return totalScore; }

void Player::changePosition(int newX, int newY) {
    Character::changePosition(newX, newY);
    addToScore(1);
    std::cout << "Player moved to (" << newX << ", " << newY << ")" << std::endl;
}

void Player::toggleCombatStyle() {
    if (currentCombatStyle == MELEE_STYLE) {
        currentCombatStyle = RANGED_STYLE;
        attackDamage = 8;
        std::cout << "Switched to RANGED combat! Damage: " << attackDamage << std::endl;
    } else {
        currentCombatStyle = MELEE_STYLE;
        attackDamage = 15;
        std::cout << "Switched to MELEE combat! Damage: " << attackDamage << std::endl;
    }
}

CombatStyle Player::getCurrentCombatStyle() const {
    return currentCombatStyle;
}

bool Player::isWithinRangedDistance(int targetX, int targetY) const {
    int distance = std::abs(targetX - coordX) + std::abs(targetY - coordY);
    return distance <= 3 && distance > 0;
}

SpellHand& Player::getSpellHand() { return spellHand; }
const SpellHand& Player::getSpellHand() const { return spellHand; }