#include "Player.h"
#include "GameConstants.h"

Player::Player(int initialHealth, int handSize)
    : health(initialHealth), maxHealth(initialHealth), currentAttackType(AttackType::MELEE),
      hand(handSize), enemiesKilled(0), enemiesKilledLastSpell(0) {}

int Player::getHealth() const {
    return health;
}

int Player::getMaxHealth() const {
    return maxHealth;
}

int Player::getDamage() const {
    return (currentAttackType == AttackType::MELEE) ? 
           Constants::PLAYER_MELEE_DAMAGE : 
           Constants::PLAYER_RANGED_DAMAGE;
}

bool Player::isAlive() const {
    return health > 0;
}

AttackType Player::getAttackType() const {
    return currentAttackType;
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

void Player::heal(int amount) {
    health += amount;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

void Player::switchAttackType() {
    currentAttackType = (currentAttackType == AttackType::MELEE) ? 
                       AttackType::RANGED : AttackType::MELEE;
}

PlayerHand& Player::getHand() {
    return hand;
}

const PlayerHand& Player::getHand() const {
    return hand;
}

void Player::incrementEnemyKill() {
    enemiesKilled++;
}

int Player::getEnemiesKilled() const {
    return enemiesKilled;
}

int Player::getEnemiesKilledSinceLastSpell() const {
    return enemiesKilled - enemiesKilledLastSpell;
}

bool Player::shouldReceiveSpell() const {
    return (enemiesKilled - enemiesKilledLastSpell) >= Constants::ENEMIES_KILLED_FOR_SPELL;
}

void Player::resetSpellGrantCounter() {
    enemiesKilledLastSpell = enemiesKilled;
}