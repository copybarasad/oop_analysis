#include "Player.h"
#include "SpellFactory.h"
#include "CombatMode.h"
#include <iostream>
#include <algorithm>

Player::Player(int startX, int startY) : position(startX, startY), combatMode(CombatMode::MELEE), spellHand(3) {}

void Player::initializeWithStarterSpell() {
    if (spellHand.getSpellCount() == 0) {
        spellHand.addSpell(SpellFactory::createRandomSpell());
        std::cout << "Получено стартовое заклинание!" << std::endl;
    }
}

void Player::move(int deltaX, int deltaY) {
    position.x += deltaX;
    position.y += deltaY;
}

void Player::takeDamage(int damageAmount) {
    health = std::max(0, health - damageAmount);
}

void Player::addScore(int points) {
    score += points;
}

void Player::switchCombatMode() {
    combatMode = (combatMode == CombatMode::MELEE) ? CombatMode::RANGED : CombatMode::MELEE;
}

int Player::getDamage() const {
    int baseDamage = (combatMode == CombatMode::MELEE) ? meleeDamage : rangedDamage;
    return CombatModeUtils::calculateEffectiveDamage(baseDamage, combatMode);
}

const char* Player::getCombatModeString() const {
    return CombatModeUtils::toString(combatMode);
}

int Player::getAttackRange() const {
    return CombatModeUtils::getAttackRange(combatMode);
}

void Player::spendMana(int amount) {
    mana = std::max(0, mana - amount);
}

void Player::restoreMana(int amount) {
    mana = std::min(maxMana, mana + amount);
}

bool Player::canCastSpell(const Spell& spell) const {
    return mana >= spell.getManaCost();
}