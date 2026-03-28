#include "TowerTurnProcessor.hpp"

TowerTurnProcessor::TowerTurnProcessor (Field& field, size_t id, EntityManager& manager) :
    TurnProcessor(id, manager), field(field) {}

TurnProcessor::TurnResult TowerTurnProcessor::run () {
    TurnResult turnRes;
    auto& tracker = manager.getTracker();
    
    Cell* currentCell = tracker[id];
    if (!currentCell->getEntity().isSkipping()) {
        turnRes.success = 1;
        if (processTowerTurn(currentCell)) {
            turnRes.playerDead = 1;
            return turnRes;
        }
    }
    else {
        currentCell->getEntity().restoreSkip();
    }

    return turnRes;
}

bool TowerTurnProcessor::processTowerTurn (Cell* currentCell) {
    Tower& tower = currentCell->getEntityAs<Tower>();

    auto attackRes = tower.attack(field, currentCell->getX(), currentCell->getY());

    if (attackRes.first) {
        manager.removeTrack(attackRes.second);
    }

    return (attackRes.second == 0);
}