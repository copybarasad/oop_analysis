#ifndef ALLY_TURN_PROC_H
#define ALLY_TURN_PROC_H

#include "../BaseTurnProcessor/TurnProcessor.hpp"
#include "../../../Services/MovementService/AllyMovementService/AllyMovementService.hpp"
#include "../../../Services/CombatService/CombatService.hpp"

class AllyTurnProcessor : public TurnProcessor {
    private:
    Field& field;
    AllyMovementService movementService;
    CombatService combatService;

    public:
    AllyTurnProcessor (Field& field, size_t id, EntityManager& manager);

    TurnResult run () override;

    private:
    void processAllyTurn (Cell* currentCell);
    void processFight (Cell* currentCell, Cell* targetCell);
};

#endif