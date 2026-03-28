#include "PlayerEconomy.h"
#include <iostream>

PlayerEconomy::PlayerEconomy(int startMana, int maxMana) 
    : score(0), mana(startMana), maxMana(maxMana), enemiesDefeated(0) {}

int PlayerEconomy::getScore() const { return score; }
int PlayerEconomy::getMana() const { return mana; }
int PlayerEconomy::getMaxMana() const { return maxMana; }
int PlayerEconomy::getEnemiesDefeated() const { return enemiesDefeated; }

void PlayerEconomy::addScore(int pts) { score += pts; }

void PlayerEconomy::addMana(int amount) { 
    mana += amount; 
    if (mana > maxMana) mana = maxMana;
    if (mana < 0) mana = 0;
}

void PlayerEconomy::addEnemyDefeated() { 
    enemiesDefeated++; 
}

bool PlayerEconomy::spendMana(int amount) {
    if (mana >= amount) {
        mana -= amount;
        return true;
    }
    return false;
}

bool PlayerEconomy::spendScore(int amount) {
    if (score >= amount) {
        score -= amount;
        return true;
    }
    return false;
}

void PlayerEconomy::setScore(int newScore) {
    score = newScore;
}

void PlayerEconomy::setMana(int newMana) {
    mana = newMana;
    if (mana > maxMana) mana = maxMana;
    if (mana < 0) mana = 0;
}