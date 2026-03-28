#ifndef COMBATSYSTEM_H
#define COMBATSYSTEM_H

#include "CombatMode.h"
#include "Position.h"
#include <vector>

class Player;
class Enemy;
class GameField;

class CombatSystem {
public:
    static bool performMeleeAttack(Player& player, const Position& target,
                                  std::vector<Enemy>& enemies);
    
    static bool performRangedAttack(Player& player, const std::vector<Enemy>& enemies,
                                   const GameField& field);
    
    static void performEnemyAttack(Enemy& enemy, Player& player);
    static bool canAttack(const Position& attackerPos, const Position& targetPos, 
                         CombatMode mode, const GameField& field);
    static bool hasLineOfSight(const Position& from, const Position& to, const GameField& field);
    
private:
    static Position findNearestEnemy(const Position& playerPos, const std::vector<Enemy>& enemies,
                                    const GameField& field);
    
    CombatSystem() = delete;
};

#endif