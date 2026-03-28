#ifndef GAME_SYSTEMS_H
#define GAME_SYSTEMS_H

#include "game_controller.h"
#include <vector>

class EnemyAI {
public:
    static void processEnemyTurns(std::vector<Enemy>& enemies, Player& player, const GameField& field, GameController& game);
    static void moveEnemyTowardsPlayer(Enemy& enemy, Player& player, const GameField& field, const std::vector<Enemy>& allEnemies, GameController& game);
};

class CombatSystem {
public:
    static bool performMeleeAttack(Player& player, std::vector<Enemy>& enemies, int targetX, int targetY);
    static bool performRangedAttack(Player& player, std::vector<Enemy>& enemies, const GameField& field, int directionX, int directionY);
};

class GameStateChecker {
public:
    static bool isGameWon(const std::vector<Enemy>& enemies, bool gameRunning, const Player& player);
    static bool areAllEnemiesDefeated(const std::vector<Enemy>& enemies);
};

#endif