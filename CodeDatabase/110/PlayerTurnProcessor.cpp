#include "PlayerTurnProcessor.hpp"

PlayerTurnProcessor::PlayerTurnProcessor (size_t id, EntityManager& manager) :
    TurnProcessor(id, manager), action("") {}

void PlayerTurnProcessor::setAction (std::string action) { this->action = action; }
    
void PlayerTurnProcessor::setChangeableFields (ChangeableFields* changeableFields) { this->changeableFields =  changeableFields; }

TurnProcessor::TurnResult PlayerTurnProcessor::run () {
    TurnResult turnRes;
    auto& tracker = manager.getTracker();
    
    Cell* currentCell = tracker[id];
    if (!currentCell->getEntity().isSkipping()) {
        processPlayerTurn(turnRes);
        turnRes.success = true;
        delete changeableFields;
        return turnRes;
    }
    currentCell->getEntity().restoreSkip();
    delete changeableFields;
    return turnRes;
}

void PlayerTurnProcessor::processPlayerTurn (TurnResult& turnRes) {
    if (action == "move") turnRes.playerDead = processMove();
    if (action == "cast") turnRes.castResult = processCast();
    if (action == "switch") turnRes.switchResult = processSwitch();
    if (action == "buy") turnRes.buyResult = processBuy();
}

bool PlayerTurnProcessor::processMove () {
    auto& tracker = manager.getTracker();
    Cell* currentCell = tracker[0];

    MoveFields* mf = dynamic_cast<MoveFields*>(changeableFields);
    MovementService::MoveResult moveRes = movementService.move(mf->getField(), *currentCell, mf->getTarget().first, mf->getTarget().second);
    bool playerKilled = 0;

    if (moveRes.success) {
        if (moveRes.slowTriggered) {
            currentCell->getEntity().setSkip();
        }
        if (moveRes.newCell) {
            manager.updateTrack(moveRes.newCell->getEntity().getID(), *moveRes.newCell);
        }
        if (moveRes.fight) {
            playerKilled = processFight(currentCell, moveRes.opponent);
        }
    }

    return playerKilled;
}

bool PlayerTurnProcessor::processFight (Cell* currentCell, Cell* targetCell) {
    Player& player = currentCell->getEntityAs<Player>();
    Entity& enemy = targetCell->getEntity();

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
    else {
        combatRes = combatService.performRangedAttack(player, enemy);
        if (combatRes.success) {
            for (auto& destroyed : combatRes.destroyedEntities) {
                manager.removeTrack(destroyed);
            }
        }
    }

    return playerKilled;
}

bool PlayerTurnProcessor::processCast () {
    CastFields* cf = dynamic_cast<CastFields*>(changeableFields);
    initializeCastService();

    bool res = castService.castSpell(*changeableFields);
    if (res) {
        cf->getHand().removeSpell(cf->getSpellName());
    }

    return !res;
}

bool PlayerTurnProcessor::processSwitch () {
    SwitchFields* sf = dynamic_cast<SwitchFields*>(changeableFields);

    CombatMode mode = manager.getPlayerPos().getEntityAs<Player&>().getMode();
    if ((mode == CombatMode::RANGED && sf->getChosenType() == "ranged") ||
        (mode == CombatMode::CLOSE && sf->getChosenType() == "close")) {
        return false;
    }

    manager.getPlayerPos().getEntityAs<Player>().setMode(sf->getChosenType());
    return true;
}

bool PlayerTurnProcessor::processBuy () {
    BuySpellFields* bsf = dynamic_cast<BuySpellFields*>(changeableFields);

    bool res = buySpellService.performSpellBuy(bsf->getStore(), bsf->getHand(), manager, bsf->getIndex());
    return res;
}

void PlayerTurnProcessor::initializeCastService () {
    CastFields* cf = dynamic_cast<CastFields*>(changeableFields);

    castService.registerFactory("direct", new DirectSpellFactory(20, 5));
    castService.registerFactory("area", new AreaSpellFactory(10, 8));
    castService.registerFactory("trap", new TrapSpellFactory(3));
    castService.registerFactory("ally", new AllySpellFactory(4));
    castService.registerFactory("buff", new BuffSpellFactory());

    castService.registerProcessor("direct", new DamageCastResultProcessor(manager, cf->getHand()));
    castService.registerProcessor("area", new DamageCastResultProcessor(manager, cf->getHand()));
    castService.registerProcessor("trap", new SpawnCastResultProcessor(manager, cf->getHand()));
    castService.registerProcessor("ally", new SpawnCastResultProcessor(manager, cf->getHand()));
}