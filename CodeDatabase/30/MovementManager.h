#pragma once
#include "Enums.h"
#include "Position.h"
#include "IGameContext.h"
#include "EntityManager.h"
#include "CombatManager.h"
#include "Field.h"

class MovementManager {
public:
    MovementManager(EntityManager& em, CombatManager& combat, Field& field)
        : em_(em), combat_(combat), field_(field) {}
    bool tryMove(EntityId id, Direction dir);
private:
    static Position stepFrom(const Position& p, Direction d);

    EntityManager& em_;
    CombatManager& combat_;

    Field& field_;
};
