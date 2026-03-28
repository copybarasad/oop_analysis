#ifndef ENEMY_H
#define ENEMY_H

#include "Unit.h"
#include "Position.h"
#include <vector>
#include <memory>

class Player;
class AllyUnit;
class EnemyBuilding;
class GameField;

class Enemy : public Unit {
public:
    Enemy(int x, int y);

    void moveAI(const Position& playerPos,
            const std::vector<std::shared_ptr<AllyUnit>>& allAllies,
            const std::vector<std::shared_ptr<Enemy>>& allEnemies,
            const std::vector<std::shared_ptr<EnemyBuilding>>& allBuildings,
            GameField& field);

    bool canAttackTarget(const Position& targetPos) const;

    CellType getCellType() const override;

private:
    Position findBestMove(const Position& targetPos,
                         const std::vector<std::shared_ptr<Enemy>>& allEnemies,
                         const std::vector<std::shared_ptr<EnemyBuilding>>& allBuildings,
                         const std::vector<std::shared_ptr<AllyUnit>>& allAllies,
                         const GameField& field);
};

#endif