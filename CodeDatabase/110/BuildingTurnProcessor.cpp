#include "BuildingTurnProcessor.hpp"

BuildingTurnProcessor::BuildingTurnProcessor (Field& field, size_t id, EntityManager& manager) :
    TurnProcessor(id, manager), field(field) {}

TurnProcessor::TurnResult BuildingTurnProcessor::run () {
    TurnResult turnRes;
    auto& tracker = manager.getTracker();
    
    Cell* currentCell = tracker[id];
    if (!currentCell->getEntity().isSkipping()) {
        turnRes.success = 1;
        processBuildingTurn(currentCell);
        turnRes.playerDead = 1;
        return turnRes;
    }
    else {
        currentCell->getEntity().restoreSkip();
        currentCell->getEntityAs<Building>().subCoolDown();
    }

    return turnRes;
}

void BuildingTurnProcessor::processBuildingTurn (Cell* currentCell) {
    Building& building = currentCell->getEntityAs<Building>();

    Enemy* enemy = building.spawnEnemy(300);

    Cell* spawnPosition = findSpawnPosition(currentCell);
    if (spawnPosition) {
        spawnPosition->setEntity(*enemy);
        manager.addTrack(spawnPosition);
    }
}

Cell* BuildingTurnProcessor::findSpawnPosition (Cell* currentCell) {
    for (int y = std::max(0, (int)currentCell->getY()-2); y <= std::min((int)field.getHeight(), (int)currentCell->getY()+2); y++) {
        for (int x = std::max(0, (int)currentCell->getX()-2); x <= std::min((int)field.getWidth(), (int)currentCell->getX()+2); x++) {
            if (field.getCell(x, y).isEmpty() && !field.getCell(x, y).isBlocked()) {
                return &field.getCell(x, y);
            }
        }
    }
    return nullptr;
}