#ifndef ISPELLFIELD_H
#define ISPELLFIELD_H

#include <vector>
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Pet.h"
#include "Cell.h"

class Player;

class ISpellField {
public:
    virtual ~ISpellField() = default;

    virtual Player& getPlayer() = 0;
    virtual const Player& getPlayer() const = 0;

    virtual bool isValidPosition(int x, int y) const = 0;
    virtual bool isPositionPassable(int x, int y) const = 0;

    virtual std::vector<Enemy>& getEnemies() = 0;
    virtual std::vector<EnemyBuilding>& getBuilding() = 0;
    virtual std::vector<Pet>& getPets() = 0;
    virtual std::vector<std::vector<Cell>>& getCells() = 0;
    
    virtual void removeDeadEntities() = 0;
};

#endif

