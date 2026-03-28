#include "Player.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include <random>
#include <iostream>


Player::Player(int initialHealth, int meleeDmg, int rangedDmg, int initialMana, int spellHandSize)
    : health(initialHealth), 
      maxHealth(initialHealth), 
      meleeDamage(meleeDmg),
      rangedDamage(rangedDmg), 
      currentCombatMode(MELEE), 
      score(100),
      mana(initialMana),
      maxMana(initialMana),
      spellHand(spellHandSize) {
    
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 2);
    int randomValue = dist(gen);

    if (randomValue == 0) {
        spellHand.addSpell(std::make_unique<DirectDamageSpell>(20, 3, 15));
    } else if(randomValue == 1) {
        spellHand.addSpell(std::make_unique<AreaDamageSpell>(15, 2, 20));
    } else {
        spellHand.addSpell(std::make_unique<TrapSpell>(25, 2, 10));
    }
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

void Player::switchCombatMode() {
    currentCombatMode = (currentCombatMode == MELEE) ? RANGED : MELEE;
}

bool Player::isAlive() const {
    return health > 0;
}

int Player::getCurrentDamage() const {
    return (currentCombatMode == MELEE) ? meleeDamage : rangedDamage;
}

int Player::getHealth() const {
    return health;
}

int Player::getScore() const {
    return score;
}

void Player::addScore(int points) {
    score += points;
}

CombatMode Player::getCombatMode() const {
    return currentCombatMode;
}

void Player::setHealth(int newHealth) {
    health = newHealth;
    if (health < 0) {
        health = 0;
    }
    if (health > maxHealth) {
        health = maxHealth;
    }
}

void Player::setScore(int newScore) {
    score = newScore;
}

void Player::setMana(int newMana) {
    mana = newMana;
    if (mana < 0) {
        mana = 0;
    }
    if (mana > maxMana) {
        mana = maxMana;
    }
}

void Player::setCombatMode(CombatMode mode) {
    currentCombatMode = mode;
}

bool Player::castSpell(int spellIndex, const Position& targetPosition, GameEngine& game) {
    int manaCost = spellHand.getSpellManaCost(spellIndex);
    if (mana < manaCost) {
        return false;
    }
    
    if (spellHand.useSpell(spellIndex, targetPosition, game)) {
        mana -= manaCost;
        return true;
    }
    
    return false;
}


void Player::addMana(int amount) {
    mana += amount;
}

int Player::getMana() const {
    return mana;
}

int Player::getMaxMana() const {
    return maxMana;
}

SpellHand& Player::getSpellHand() {
    return spellHand;
}

const SpellHand& Player::getSpellHand() const {
    return spellHand;
}

void Player::onEnemyKilled() {
    addMana(20);
    addScore(100);
}

bool Player::canBuySpell() const {
    return score >= 50 && spellHand.getSpellCount() < spellHand.getMaxSize();
}

void Player::buyRandomSpell() {
    if (!canBuySpell()) {
        return;
    }
    
    bool hasDirectDamage = false;
    bool hasAreaDamage = false;
    bool hasTrapDamage = false;
    
    for (int i = 0; i < spellHand.getSpellCount(); ++i) {
        std::string name = spellHand.getSpellName(i);
        if (name == "Direct Damage") hasDirectDamage = true;
        if (name == "Area Damage") hasAreaDamage = true;
        if (name == "Trap Damage") hasTrapDamage = true;
    }
    
    if (hasDirectDamage && hasAreaDamage&&hasTrapDamage) {
        return;
    }
    
    std::unique_ptr<Spell> newSpell;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 2);
    int randomValue = dist(gen);
     if (!hasDirectDamage) {
        newSpell = std::make_unique<DirectDamageSpell>(20, 3, 15);
    } else if(!hasAreaDamage) {
        newSpell = std::make_unique<AreaDamageSpell>(15, 2, 20);
    } else {
        newSpell = std::make_unique<TrapSpell>(25, 2, 10);
    }
    if (spellHand.addSpell(std::move(newSpell))) {
        score -= 50;
    }
}