#ifndef COMBATSYSTEM_H
#define COMBATSYSTEM_H

#include "AttackType.h"
#include "RangedAttackLogic.h"
#include "PlayerStats.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <type_traits>

// Forward declarations
class Enemy;
class EnemyTower;
class GameField;

class CombatSystem {
private:
    int m_meleeDamage;
    int m_rangedDamage;
    AttackType m_currentAttackType;

    template<typename TargetType>
    void performAttackOnTargets(std::vector<TargetType>& targets,
        const GameField& field,
        PlayerStats& stats,
        int playerX,
        int playerY,
        int damage);

public:
    CombatSystem(int meleeDamage, int rangedDamage)
        : m_meleeDamage(meleeDamage)
        , m_rangedDamage(rangedDamage)
        , m_currentAttackType(AttackType::Melee) {}

    CombatSystem(const CombatSystem&) = delete;
    CombatSystem& operator=(const CombatSystem&) = delete;

    CombatSystem(CombatSystem&& other) noexcept = default;
    CombatSystem& operator=(CombatSystem&& other) noexcept = default;

    void attack(std::vector<Enemy>& enemies,
        std::vector<EnemyTower>& towers,
        const GameField& field,
        PlayerStats& stats,
        int playerX,
        int playerY);

    void switchAttackType();
    AttackType getCurrentAttackType() const;
    int getCurrentDamage() const;
    int getMeleeDamage() const;
    int getRangedDamage() const;
};

// Реализация шаблона должна быть в заголовочном файле
template<typename TargetType>
void CombatSystem::performAttackOnTargets(std::vector<TargetType>& targets,
    const GameField& field,
    PlayerStats& stats,
    int playerX,
    int playerY,
    int damage) {

    for (auto& target : targets) {
        if (!target.isAlive()) continue;

        bool canAttack = false;
        if (m_currentAttackType == AttackType::Melee) {
            int dx = std::abs(target.getPositionX() - playerX);
            int dy = std::abs(target.getPositionY() - playerY);
            canAttack = (dx <= 1 && dy <= 1);
        }
        else {
            canAttack = RangedAttackLogic::isInRange(playerX, playerY,
                target.getPositionX(), target.getPositionY(), 3) &&
                RangedAttackLogic::isPathClear(playerX, playerY,
                    target.getPositionX(), target.getPositionY(), field);
        }

        if (canAttack) {
            target.takeDamage(damage);
            std::cout << "Player hits target for " << damage << " damage!" << std::endl;

            // Награда за убийство
            if (!target.isAlive()) {
                if constexpr (std::is_same_v<TargetType, Enemy>) {
                    stats.addScore(100);
                    stats.addMana(25);
                }
            }
        }
    }
}

#endif // COMBATSYSTEM_H