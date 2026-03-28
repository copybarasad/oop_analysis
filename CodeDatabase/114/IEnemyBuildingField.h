#ifndef IENEMYBUILDINGFIELD_H
#define IENEMYBUILDINGFIELD_H

#include <vector>
#include "EnemyBuilding.h"
#include "Enemy.h"
#include "Cell.h"

class IEnemyBuildingField {
public:
    virtual ~IEnemyBuildingField() = default;

    virtual std::vector<EnemyBuilding>& getBuilding() = 0;
    virtual std::vector<Enemy>& getEnemies() = 0;
    virtual bool isValidPosition(int x, int y) const = 0;
    virtual bool isPositionEmpty(int x, int y) const = 0;
    virtual std::vector<std::vector<Cell>>& getCells() = 0;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
};

#endif

