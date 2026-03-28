#ifndef ALLY_UNIT_H
#define ALLY_UNIT_H

#include "Unit.h"
#include "Position.h"
#include <vector>
#include <memory>

class Player;
class Enemy;
class EnemyBuilding;
class GameField;

class AllyUnit : public Unit {
public:
    AllyUnit(int x, int y);

    void moveAI(const std::vector<std::shared_ptr<Enemy>>& enemies,
                const std::vector<std::shared_ptr<EnemyBuilding>>& buildings,
                const GameField& field);

    bool canAttackTarget(const Position& targetPos) const;

    CellType getCellType() const override;
};

#endif