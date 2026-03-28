#include "Player.h"

Player::Player(int x, int y,int maxHealth, int meleeDamage, int rangedDamage) 
    : Character(x,y,maxHealth),slowed(false),
      maxHealth(maxHealth), 
      score(0), 
      meleeDamage(meleeDamage), 
      rangedDamage(rangedDamage), 
      coins(0),
      combatMode(CombatMode::MELEE),hand(){}

int Player::getScore() const {
    return score;
}

int Player::getDamage() const {
    switch (combatMode) {
        case CombatMode::MELEE:
            return meleeDamage;
        case CombatMode::RANGED:
            return rangedDamage;
        default:
            return meleeDamage;
    }
}

int Player::getCoins() const{
    return coins;
}

void Player::updateCoins(int points){
    coins += points;
}

Player::CombatMode Player::getCombatMode() const {
    return combatMode;
}

void Player::addScore(int score_points) {
    score += score_points;
}

void Player::switchCombatMode() {
    if (combatMode == CombatMode::MELEE) {
        combatMode = CombatMode::RANGED;
    } else {
        combatMode = CombatMode::MELEE;
    }
}

const std::pair<int, int> Player::getPlayerDamages() const{
    return {meleeDamage,rangedDamage};
}

bool Player::isSlowed() const { return slowed; }

void Player::getSlowedDown() { slowed = true; }

void Player::removeSlow() { slowed = false; }

std::string Player::combatModeToString(const CombatMode& mode) const{
    if (mode == CombatMode::MELEE){
        return "MELEE";
    }
    return "RANGED";
}

Player::CombatMode Player::stringToCombatMode(const std::string& str) {
        if (str == "MELEE") return CombatMode::MELEE;
        if (str == "RANGED") return CombatMode::RANGED;
        return CombatMode::MELEE;
}