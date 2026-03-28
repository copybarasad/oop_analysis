#include "CombatSystem.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include "GameField.h"
#include <iostream>

void CombatSystem::attack(std::vector<Enemy>& enemies,
    std::vector<EnemyTower>& towers,
    const GameField& field,
    PlayerStats& stats,
    int playerX,
    int playerY) {

    int damage = getCurrentDamage();
    int hitCount = 0;

    performAttackOnTargets(enemies, field, stats, playerX, playerY, damage);
    performAttackOnTargets(towers, field, stats, playerX, playerY, damage);

    for (const auto& enemy : enemies) {
        if (!enemy.isAlive()) {
            hitCount++;
        }
    }

    for (const auto& tower : towers) {
        if (!tower.isAlive()) {
            hitCount++;
        }
    }

    if (hitCount == 0) {
        std::cout << "Attack: no targets in range!" << std::endl;
    }
    else {
        std::cout << "Attack! Hit " << hitCount << " targets." << std::endl;
    }
}

void CombatSystem::switchAttackType() {
    m_currentAttackType = (m_currentAttackType == AttackType::Melee)
        ? AttackType::Ranged
        : AttackType::Melee;
}

AttackType CombatSystem::getCurrentAttackType() const {
    return m_currentAttackType;
}

int CombatSystem::getCurrentDamage() const {
    return (m_currentAttackType == AttackType::Melee) ? m_meleeDamage : m_rangedDamage;
}

int CombatSystem::getMeleeDamage() const {
    return m_meleeDamage;
}

int CombatSystem::getRangedDamage() const {
    return m_rangedDamage;
}