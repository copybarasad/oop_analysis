#include "Player.h"
#include "Spell.h"
#include "Field.h"
#include "DirectDamageSpell.h"
#include "Renderer.h"

#include <algorithm>
#include <iostream>

Player::Player(int startHealth, int meleeDmg, int rangedDmg)
    : health(startHealth), maxHealth(startHealth), 
      meleeDamage(meleeDmg), rangedDamage(rangedDmg),
      score(0), combatType(CombatType::MELEE), canMove(true),
      spellHand(3),  
      mana(50), maxMana(100)
{
}


int Player::getHealth() const { return health; }
int Player::getMaxHealth() const { return maxHealth; }
int Player::getDamage() const { 
    return (combatType == CombatType::MELEE) ? meleeDamage : rangedDamage; 
}
int Player::getMeleeDamage() const { return meleeDamage; }  
int Player::getRangedDamage() const { return rangedDamage; } 
int Player::getScore() const { return score; }
CombatType Player::getCombatType() const { return combatType; }
bool Player::canMoveNextTurn() const { return canMove; }



void Player::takeDamage(int damage) { 
    health = std::max(0, health - damage); 
}

void Player::addScore(int points) { 
    score += points; 

    if (score % 100 == 0) {
        restoreMana(20);
    }
}

void Player::heal(int amount) { 
    health = std::min(maxHealth, health + amount); 
}

void Player::switchCombatType() {
    combatType = (combatType == CombatType::MELEE) ? CombatType::RANGED : CombatType::MELEE;
    canMove = false;
}

void Player::resetMovement() {
    canMove = true;
}

void Player::setMovement(bool movement) {
    canMove = movement;
}

bool Player::isAlive() const {
    return health > 0;
}



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


void Player::setMana(int newMana) {
    mana = std::max(0, std::min(newMana, maxMana));
}

// Восстановление маны
void Player::restoreMana(int amount) {
    if (amount > 0) {
        mana = std::min(mana + amount, maxMana);
    }
}

// Проверка, может ли игрок применить заклинание
bool Player::canCastSpell(const Spell& spell) const {
    return mana >= spell.getManaCost() && health > 0;
}

// Покупка заклинания за очки
bool Player::buySpell(std::unique_ptr<Spell> spell, int cost) {
    if (spell == nullptr) {
        return false;
    }
    
    // Проверяем, хватает ли очков
    if (score < cost) {
        return false;
    }
    
    // Пытаемся добавить заклинание в руку
    if (spellHand.addSpell(std::move(spell))) {
        // Вычитаем стоимость из очков
        score -= cost;
        return true;
    }
    
    return false;
}

bool Player::castSpell(int spellIndex, GameField& field, int targetX, int targetY) {
    // Проверяем валидность индекса
    if (spellIndex < 0 || spellIndex >= spellHand.getSpellCount()) {
        return false;
    }
    
    // Получаем заклинание
    Spell* spell = spellHand.getSpell(spellIndex - 1);
    if (!spell) {
        return false;
    }
    
    // Проверяем, может ли игрок применить заклинание
    if (!canCastSpell(*spell)) {
        return false;
    }
    
    // Применяем заклинание
    return spell->cast(field, *this, targetX, targetY);
}


bool Player::hasSpells() const {
    return spellHand.getSpellCount() > 0;
}


void Player::setHealth(int hp) {
    health = std::min(maxHealth, std::max(0, hp));
}

void Player::setMaxHealth(int maxHp) {
    maxHealth = maxHp;
    if (health > maxHealth) health = maxHealth;
}

void Player::setMaxMana(int maxMp) {
    maxMana = maxMp;
    if (mana > maxMana) mana = maxMana;
}

void Player::setScore(int newScore) {
    score = newScore;
}

void Player::setCombatType(CombatType type) {
    combatType = type;
}

void Player::clearSpells() {
    spellHand.clear();
}

void Player::setMeleeDamage(int damage) {
    meleeDamage = damage;
}

void Player::setRangedDamage(int damage) {
    rangedDamage = damage;
}

void Player::displaySpellInfo() const {
    GameRenderer renderer;
    renderer.renderPlayerSpellInfo(*this);
}