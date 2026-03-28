#include "AllyTurnProcessor.hpp"

AllyTurnProcessor::AllyTurnProcessor (Field& field, size_t id, EntityManager& manager) :
    TurnProcessor(id, manager), field(field) {}

TurnProcessor::TurnResult AllyTurnProcessor::run () {
    TurnResult turnRes;
    auto& tracker = manager.getTracker();
    
    Cell* currentCell = tracker[id];
    if (!currentCell->getEntity().isSkipping()) {
        turnRes.success = 1;
        processAllyTurn(currentCell);
        turnRes.playerDead = 1;
        return turnRes;
    }
    else {
        currentCell->getEntity().restoreSkip();
    }

    return turnRes;
}

void AllyTurnProcessor::processAllyTurn (Cell* currentCell) {
    Ally& ally = currentCell->getEntityAs<Ally>();
    Cell& playerPosition = manager.getPlayerPos();

    MovementService::MoveResult moveRes = movementService.move (field, *currentCell, playerPosition.getX(), playerPosition.getY());

    if (moveRes.success) {
        if (moveRes.slowTriggered) {
            currentCell->getEntity().setSkip();
        }
        if (moveRes.fight) {
            processFight(currentCell, moveRes.opponent);
        }
        if (moveRes.newCell) {
            manager.updateTrack(ally.getID(), *moveRes.newCell);
        }
    }
}

void AllyTurnProcessor::processFight (Cell* currentCell, Cell* targetCell) {
    Player& player = manager.getPlayerPos().getEntityAs<Player>();
    Ally& ally = currentCell->getEntityAs<Ally>();
    Entity& enemy = targetCell->getEntity();

    CombatService::CombatResult combatRes = combatService.performNpcFight(player, ally, enemy);

    if (combatRes.success) {
        for (auto& destroyed : combatRes.destroyedEntities) {
            manager.removeTrack(destroyed);
        }
    }
}