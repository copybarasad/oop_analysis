#include "EnemyTurnProcessor.hpp"

EnemyTurnProcessor::EnemyTurnProcessor (Field& field, size_t id, EntityManager& manager) :
    TurnProcessor(id, manager), field(field) {}

TurnProcessor::TurnResult EnemyTurnProcessor::run () {
    TurnResult turnRes;
    auto& tracker = manager.getTracker();
    
    Cell* currentCell = tracker[id];
    if (!currentCell->getEntity().isSkipping()) {
        turnRes.success = 1;
        if (processEnemyTurn(currentCell)) {
            turnRes.playerDead = 1;
            return turnRes;
        }
    }
    else {
        currentCell->getEntity().restoreSkip();
    }

    return turnRes;
}

bool EnemyTurnProcessor::processEnemyTurn (Cell* currentCell) {
    Enemy& enemy = currentCell->getEntityAs<Enemy>();
    auto playerPosition = manager.getPlayerPos();

    MovementService::MoveResult moveRes = movementService.move(field, *currentCell, playerPosition.getX(), playerPosition.getY());
    bool playerKilled = 0;

    if (moveRes.success) {
        if (moveRes.slowTriggered) {
            currentCell->getEntity().setSkip();
        }
        if (moveRes.newCell) {
            manager.updateTrack(enemy.getID(), *moveRes.newCell);
        }
        if (moveRes.fight) {
            playerKilled = processFight(currentCell);
        }
    }

    return playerKilled;
}

bool EnemyTurnProcessor::processFight (Cell* currentCell) {
    Cell& playerPosition = manager.getPlayerPos();
    Player& player = playerPosition.getEntityAs<Player>();
    Enemy& enemy = currentCell->getEntityAs<Enemy>();
    
    CombatService::CombatResult combatRes = combatService.performCloseFight(player, enemy);
    bool playerKilled = 0;

    if (combatRes.success) {
        for (auto& destroyed : combatRes.destroyedEntities) {
            manager.removeTrack(destroyed);
            if (destroyed == 0) {
                playerKilled = 1;
            }
        }
    }

    return playerKilled;
}