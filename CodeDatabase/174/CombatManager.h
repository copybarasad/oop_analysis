#ifndef COMBAT_MANAGER_H
#define COMBAT_MANAGER_H

#include "Player.h"
#include <vector>
#include <memory>

class Enemy;
class EnemyBuilding;

class CombatManager {
public:
    bool performPlayerAttack(Player& player, 
                           std::vector<std::shared_ptr<Enemy>>& enemies,
                           std::vector<std::shared_ptr<EnemyBuilding>>& buildings) const;
    
    bool resolveEnemyAttacks(Player& player, const std::vector<std::shared_ptr<Enemy>>& enemies) const;

    bool tryAttackEnemy(Player& player, std::shared_ptr<Enemy>& enemy, int range) const;
    bool tryAttackBuilding(Player& player, std::shared_ptr<EnemyBuilding>& building, int range) const;
    bool isInAttackRange(const Position& attackerPos, const Position& targetPos, int range) const;
};

#endif