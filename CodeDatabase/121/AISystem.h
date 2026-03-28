#ifndef AI_SYSTEM_H
#define AI_SYSTEM_H

#include "Player.h"
#include "Enemy.h"
#include "GameField.h"
#include "EnemyBuilding.h"
#include "Position.h"
#include <vector>

class AISystem {
public:
    AISystem() = default;

    void processEnemiesTurn(std::vector<Enemy>& enemies, Player& player, GameField& field);
    void spawnEnemyFromBuilding(EnemyBuilding& building, std::vector<Enemy>& enemies, GameField& field);

private:
    void processEnemyBehavior(Enemy& enemy, Player& player, GameField& field);
    bool checkJockerTrap(Enemy& enemy, GameField& field);
    void executeAttack(Enemy& enemy, Player& player);
    bool tryMovement(Enemy& enemy, const Player& player, GameField& field);
    bool performMovement(Enemy& enemy, const Position& newPos, GameField& field);
    bool tryAlternativeMovements(Enemy& enemy, const Player& player, GameField& field);
    bool shouldAttackPlayer(const Enemy& enemy, const Player& player) const;
    Position calculateMovementDirection(const Enemy& enemy, const Player& player) const;
    bool isValidMovementPosition(const Position& newPos, const GameField& field) const;
    Position selectSpawnPosition(const std::vector<Position>& positions) const;
};

#endif