#ifndef IENEMYFIELD_H
#define IENEMYFIELD_H

#include <vector>
#include "Enemy.h"
#include "Cell.h"

class IEnemyField {
public:
    virtual ~IEnemyField() = default;

    virtual std::vector<Enemy>& getEnemies() = 0;
    virtual bool isValidPosition(int x, int y) const = 0;
    virtual bool isPositionEmpty(int x, int y) const = 0;
    virtual std::vector<std::vector<Cell>>& getCells() = 0;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
};

#endif

