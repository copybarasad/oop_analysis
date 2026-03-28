#include "Player.h"
#include <cmath>
#include <iostream>

Player::Player(int health, int meleeDmg, int rangedDmg, int mana, int maxMana) 
    : health(health), maxHealth(health), meleeDamage(meleeDmg), 
      rangedDamage(rangedDmg), score(0), pos(0, 0), alive(true), 
      attackType(AttackType::MELEE),
      spellHand(5), mana(mana), maxMana(maxMana), invisibilityTurns(0), enhancementPower(0) {
}

int Player::getHealth() const { return health; }
int Player::getDamage() const { 
    return (attackType == AttackType::MELEE) ? meleeDamage : rangedDamage; 
}
int Player::getMeleeDamage() const { return meleeDamage; }
int Player::getRangedDamage() const { return rangedDamage; }
int Player::getScore() const { return score; }
const Position& Player::getPos() const { return pos; }
bool Player::isAlive() const { 
    return alive; 
}
AttackType Player::getAttackType() const { return attackType; }

void Player::takeDamage(int damage) {
    if (damage > 0) {
        health -= damage;
        checkAlive();
    }
}

void Player::addScore(int points) {
    if (points > 0) score += points;
}

void Player::move(int dx, int dy) {
    pos.move(dx, dy);
}

void Player::setPos(int x, int y) {
    pos.set(x, y);
}

void Player::switchAttackType() {
    attackType = (attackType == AttackType::MELEE) ? AttackType::RANGED : AttackType::MELEE;
}

void Player::checkAlive() {
    alive = (health > 0);
}

bool Player::canAttack(const Position& targetPos, AttackType type) const {
    int dx = std::abs(pos.getX() - targetPos.getX());
    int dy = std::abs(pos.getY() - targetPos.getY());
    
    if (type == AttackType::MELEE) {
        // расстояние 1 по горизонтали или вертикали
        return (dx == 1 && dy == 0) || (dx == 0 && dy == 1);
    } else {
        // расстояние 2 по горизонтали или вертикали (через одну клетку)
        return (dx == 2 && dy == 0) || (dx == 0 && dy == 2);
    }
}

// Заклинания - ТОЛЬКО ОДНА РЕАЛИЗАЦИЯ КАЖДОГО МЕТОДА
SpellHand& Player::getSpellHand() {
    return spellHand;
}

const SpellHand& Player::getSpellHand() const {
    return spellHand;
}

int Player::getMana() const { 
    return mana; 
}

int Player::getMaxMana() const { 
    return maxMana; 
}

int Player::getInvisibilityTurns() const {
    return invisibilityTurns;
}

void Player::addMana(int amount) {
    mana += amount;
    if (mana > maxMana) mana = maxMana;
    if (mana < 0) mana = 0;
}

void Player::setInvisibilityTurns(int turns) {
    invisibilityTurns = turns;
}

void Player::decrementInvisibility() {
    if (invisibilityTurns > 0) {
        invisibilityTurns--;
    }
}

bool Player::canCastSpell() const {
    return true;
}

void Player::castSpell(const Position& targetPos) {
    // Реализация метода castSpell
}