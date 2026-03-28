#include "GameLevel.hpp"

GameLevel::GameLevel (std::pair<size_t, size_t> fieldSize, std::pair<double, double> fieldInit, size_t spellNumber, std::string baseSpell,
    std::list<std::pair<Entity*, std::pair<size_t, size_t>>> entitiesList, std::vector<std::string> storeSpells, size_t currentTurnID) :
    field(fieldSize.first, fieldSize.second, fieldInit.first, fieldInit.second), hand(spellNumber, baseSpell), store(storeSpells),
    playerTurn(0, manager), allyTurn(field, 100, manager), enemyTurn(field, 300, manager), buildingTurn(field, 400, manager), towerTurn(field, 500, manager),
    currentTurnID(currentTurnID), levelCompleted(false), playerAlive(true) {
    setEntities(entitiesList);
    pendingPlayerAction = "";
    pendingChangeableFields = nullptr;
}

GameLevel::~GameLevel () {
    delete pendingChangeableFields;
    manager.deleteEntities();
}

void GameLevel::updateState () {
    if (levelCompleted) {
        return;
    }
    processCurrentTurn();
    checkLevelCompletion();
    checkPlayer();
}

bool GameLevel::isLevelCompleted () const { return levelCompleted; }

bool GameLevel::isPlayerAlive () const { return playerAlive; }

size_t GameLevel::getCurrentTurnID () const { return currentTurnID; }

bool GameLevel::isWaitingForPlayerInput () const { return currentTurnID == 0 && pendingPlayerAction.empty(); }

bool GameLevel::queuePlayerAction(const std::string& action, ChangeableFields* changeableFields) {
    if (currentTurnID != 0 || !pendingPlayerAction.empty()) {
        delete changeableFields;
        return false;
    }
    pendingPlayerAction = action;
    pendingChangeableFields = changeableFields;

    return true;
}

Field& GameLevel::getField () { return field; }

SpellHand& GameLevel::getHand () { return hand; }

EntityManager& GameLevel::getEntityManager() { return manager; }

SpellStore& GameLevel::getSpellStore () { return store; }

void GameLevel::setEntities (std::list<std::pair<Entity*, std::pair<size_t, size_t>>> entitiesList) {
    for (auto& [entity, coords] : entitiesList) {
        Cell& cell = field.getCell(coords.first, coords.second);
        if (cell.isBlocked()) {
            cell.setType(CellType::NORMAL);
        }
        cell.setEntity(*entity);
    }
}

void GameLevel::processCurrentTurn () {
    if (currentTurnID == 0) {
        if (!pendingPlayerAction.empty()) {
            playerTurn.setAction(pendingPlayerAction);
            playerTurn.setChangeableFields(pendingChangeableFields);

            auto result = playerTurn.run();

            if (result.playerDead) {
                levelCompleted = true;
            }
            
            advanceToNextTurn();
        }
    }
    else if (currentTurnID >= 100 && currentTurnID < 200) {
        allyTurn.setID(currentTurnID);
        allyTurn.run();
        advanceToNextTurn();
    }
    else if (currentTurnID >= 300 && currentTurnID < 400) {
        enemyTurn.setID(currentTurnID);
        auto result = enemyTurn.run();

        if (result.playerDead) {
            levelCompleted = true;
        }

        advanceToNextTurn();
    }
    else if (currentTurnID >= 400 && currentTurnID < 500) {
        buildingTurn.setID(currentTurnID);
        buildingTurn.run();
        advanceToNextTurn();
    }
    else if (currentTurnID >= 500 && currentTurnID < 600) {
        towerTurn.setID(currentTurnID);
        auto result = towerTurn.run();

        if (result.playerDead) {
            levelCompleted = true;
        }
        
        advanceToNextTurn();
    }
}

void GameLevel::advanceToNextTurn () {
    auto tracker = manager.getTracker();
    while (tracker.find(currentTurnID) == tracker.end()) {
        currentTurnID++;
        if (currentTurnID >= 600) {
            currentTurnID = 0;
            break;
        }
    }

    resetTurnStates();
}

void GameLevel::resetTurnStates() {
    pendingPlayerAction.clear();
    delete pendingChangeableFields;
    pendingChangeableFields = nullptr;
}

void GameLevel::checkLevelCompletion () {
    if (manager.getEnemiesNumber() == 0) {
        levelCompleted = true;
    }
}

void GameLevel::checkPlayer () {
    if (manager.getTracker().find(0) == manager.getTracker().end()) {
        playerAlive = false;
    }
}