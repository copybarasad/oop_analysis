#ifndef ENEMY_TURN_PROC_H
#define ENEMY_TURN_PROC_H

#include "../BaseTurnProcessor/TurnProcessor.hpp"
#include "../../../Services/MovementService/EnemyMovementService/EnemyMovementService.hpp"
#include "../../../Services/CombatService/CombatService.hpp"

class EnemyTurnProcessor : public TurnProcessor {
    private:
    Field& field;
    EnemyMovementService movementService;
    CombatService combatService;

    public:
    EnemyTurnProcessor (Field& field, size_t id, EntityManager& manager);

    TurnResult run () override;

    private:
    bool processEnemyTurn (Cell* currentCell);
    bool processFight (Cell* currentCell);
};

#endif