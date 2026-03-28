#ifndef COMBAT_SYSTEM_H
#define COMBAT_SYSTEM_H

#include "Player.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include "AttackResult.h"
#include "GameField.h"
#include <vector>

class CombatSystem {
public:
    static AttackResult performPlayerAttack(Player &player, Character &target);
    static void applyDamage(Character &target, int damage);
    static bool isInRange(const Position &attackerPos, const Position &targetPos, int range);
    static void performAreaAttack(Player &player, std::vector<Enemy> &enemies, std::vector<EnemyTower> &towers);
    static void performRangedAttack(Player &player, const Position &direction, std::vector<Enemy> &enemies, GameField &field);
};

#endif