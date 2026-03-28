#ifndef TOWER_TURN_PROC_H
#define TOWER_TURN_PROC_H

#include "../BaseTurnProcessor/TurnProcessor.hpp"
#include "../../../Entities/Inanimate/Tower/Tower.hpp"

class TowerTurnProcessor : public TurnProcessor {
    private:
    Field& field;

    public:
    TowerTurnProcessor (Field& field, size_t id, EntityManager& manager);

    TurnResult run () override;

    private:
    bool processTowerTurn (Cell* currentCell);
};

#endif