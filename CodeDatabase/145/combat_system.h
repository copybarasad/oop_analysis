#ifndef COMBAT_SYSTEM_H
#define COMBAT_SYSTEM_H

#include "player.h"
#include "enemy.h"
#include "enemy_building.h"
#include <vector>

class CombatSystem {
private:
    Player& player;
    std::vector<Enemy>& enemies;
    std::vector<EnemyBuilding>& buildings;

public:
    CombatSystem(Player& player, std::vector<Enemy>& enemies, std::vector<EnemyBuilding>& buildings);

    void performMeleeAttack();
    void performRangedAttack();
    void checkCombat();

private:
    void attackEnemy(Enemy& enemy, int damage, bool isRanged);
};

#endif