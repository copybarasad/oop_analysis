#ifndef IPLAYERFIELD_H
#define IPLAYERFIELD_H

#include <vector>
#include <optional>
#include <functional>
#include "Cell.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Entity.h"

class IPlayerField {
public:
    virtual ~IPlayerField() = default;

    virtual Player& getPlayer() = 0;
    virtual const Player& getPlayer() const = 0;

    virtual bool isValidPosition(int x, int y) const = 0;
    virtual bool isPositionPassable(int x, int y) const = 0;

    virtual std::vector<std::vector<Cell>>& getCells() = 0;
    virtual const std::vector<std::vector<Cell>>& getCells() const = 0;

    virtual std::optional<std::reference_wrapper<Entity>> findEntityAt(int x, int y) = 0;

    virtual std::vector<Enemy>& getEnemies() = 0;
    virtual std::vector<EnemyBuilding>& getBuilding() = 0;
};

#endif

