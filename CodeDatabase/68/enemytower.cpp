#include "enemytower.h"
#include "player.h"
#include "directdamagespell.h"
#include <iostream>
#include <cmath>

EnemyTower::EnemyTower(const Position& position, int attackRadius, int cooldownDuration)
    : position(position),
      attackRadius(attackRadius),
      // WEAKENED VERSION: 15 damage instead of 25, 0 mana cost
      attackSpell(std::unique_ptr<Spell>(new DirectDamageSpell(15, 0, attackRadius))),
      canAttack(true),
      cooldownTimer(0),
      cooldownDuration(cooldownDuration) {}

// Copy constructor
EnemyTower::EnemyTower(const EnemyTower& other)
    : position(other.position),
      attackRadius(other.attackRadius),
      // Create new weakened spell for copy
      attackSpell(std::unique_ptr<Spell>(new DirectDamageSpell(15, 0, other.attackRadius))),
      canAttack(other.canAttack),
      cooldownTimer(other.cooldownTimer),
      cooldownDuration(other.cooldownDuration) {}

// Copy assignment operator
EnemyTower& EnemyTower::operator=(const EnemyTower& other) {
    if (this != &other) {
        position = other.position;
        attackRadius = other.attackRadius;
        attackSpell = std::unique_ptr<Spell>(new DirectDamageSpell(15, 0, other.attackRadius));
        canAttack = other.canAttack;
        cooldownTimer = other.cooldownTimer;
        cooldownDuration = other.cooldownDuration;
    }
    return *this;
}

bool EnemyTower::attackPlayer(Player* player, GameField* field) {
    if (!player || !canAttack || !field) {
        if (!canAttack) {
            std::cout << "Tower at " << position.x << "," << position.y << " is on cooldown! (" 
                      << cooldownTimer << " turns remaining)\n";
        }
        return false;
    }

    if (!isPlayerInRange(player->getPosition())) {
        return false;
    }

    std::cout << "Tower at " << position.x << "," << position.y << " casts weakened direct damage spell!\n";
    
    // FIXED: Use spell system - damage comes from Spell class
    // Pass nullptr for GameField since tower doesn't need field validation
    attackSpell->cast(field, player->getPosition());
    
    // Lab requirement: "Не может применять заклинание несколько ходов подряд"
    canAttack = false;
    cooldownTimer = cooldownDuration;
    return true;
}

void EnemyTower::updateCooldown() {
    if (!canAttack) {
        cooldownTimer--;
        if (cooldownTimer <= 0) {
            canAttack = true;
            cooldownTimer = 0;
        }
    }
}

bool EnemyTower::isPlayerInRange(const Position& playerPosition) const {
    return position.distanceTo(playerPosition) <= attackRadius;
}

const Position& EnemyTower::getPosition() const {
    return position;
}

int EnemyTower::getAttackRadius() const {
    return attackRadius;
}

bool EnemyTower::getCanAttack() const {
    return canAttack;
}