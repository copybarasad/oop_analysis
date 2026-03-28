#ifndef BUILDING_TURN_PROC_H
#define BUILDING_TURN_PROC_H

#include "../BaseTurnProcessor/TurnProcessor.hpp"
#include "../../../Entities/Inanimate/Building/Building.hpp"

class BuildingTurnProcessor : public TurnProcessor {
    private:
    Field& field;

    public:
    BuildingTurnProcessor (Field& field, size_t id, EntityManager& manager);

    TurnResult run () override;

    private:
    void processBuildingTurn (Cell* currentCell);
    Cell* findSpawnPosition (Cell* currentCell);
};

#endif