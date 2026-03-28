#include "Player.h"
#include <cmath>

Player::Player(const Position &startPosition, int health, int meleeDmg, int rangedDmg)
    : Character(startPosition, health, meleeDmg), currentCombatType(CombatType::MELEE),
      score(0), meleeDamage(meleeDmg), rangedDamage(rangedDmg), canMove(true),
      meleeRange(1), rangedRange(3), coins(0), maxHealth(health) {}

CombatType Player::getCombatType() const { return currentCombatType; }

void Player::switchCombatType() {
    if (currentCombatType==CombatType::MELEE) { 
        currentCombatType=CombatType::RANGED; 
        damage=rangedDamage; 
    }
    else { 
        currentCombatType=CombatType::MELEE; 
        damage=meleeDamage; 
    }
}

int Player::getScore() const { return score; }
void Player::addScore(int points) { 
    if (points < 0 && score + points < 0) {
        score = 0;
    } else {
        score += points; 
    }
}
bool Player::getCanMove() const { return canMove; }
void Player::setCanMove(bool movable) { canMove = movable; }
void Player::move(const Position &newPosition) { if (canMove) position = newPosition; }
int Player::getAttackRange() const { return (currentCombatType==CombatType::MELEE) ? meleeRange : rangedRange; }

bool Player::isInAttackRange(const Position &targetPosition, const Position &enemyPosition) const {
    int distance = std::abs(targetPosition.x - enemyPosition.x) + std::abs(targetPosition.y - enemyPosition.y);
    return distance <= getAttackRange();
}

AttackResult Player::attack(Character &target) {
    if (!target.isAlive()) return AttackResult::NO_ENEMY;
    Position targetPos = target.getPosition();
    int distance = std::abs(position.x - targetPos.x) + std::abs(position.y - targetPos.y);
    int maxRange = getAttackRange();
    if (distance > maxRange) return AttackResult::OUT_OF_RANGE;
    target.takeDamage(damage);
    addScore(10);
    return AttackResult::SUCCESS;
}

int Player::getMeleeRange() const { return meleeRange; }
int Player::getRangedRange() const { return rangedRange; }
void Player::addCoins(int c) { coins += c; }
bool Player::trySpendCoins(int c) { if (coins>=c) { coins-=c; return true;} return false; }
int Player::getCoins() const { return coins; }