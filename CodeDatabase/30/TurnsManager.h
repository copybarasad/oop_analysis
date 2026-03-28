#pragma once
#include "Enums.h"
#include "MovementManager.h"
#include <vector>

class Manager;
class EntityManager; 

class TurnsManager {
public:
    TurnsManager(Manager& manager, MovementManager& movement, EntityManager& em)
        : manager_(manager), movement_(movement), em_(em) {}

    void enemyTurn();
    void buildingsTurn();
    void alliesTurn();
private:
    Direction chooseStepTowards(const Position& from, const Position& to) const;

    Manager& manager_;
    MovementManager& movement_;
    EntityManager& em_;
};
