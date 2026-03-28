#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GameField.h"
#include "Wanderer.h"
#include "BeastManager.h"
#include "BuildingManager.h"
#include "CombatManager.h"
#include "InputHandler.h"
#include "Renderer.h"
#include "GameExceptions.h"
#include <memory>
#include <vector>

class GameController {
private:
    int turnCount;
    bool gameRunning;
    bool wandererCanMove;
    bool wandererOnSlowCell;
    int currentLevel;

    GameField field;
    Wanderer wanderer;
    Position wandererPosition;
    Renderer renderer;

    std::unique_ptr<BeastManager> beastManager;
    std::unique_ptr<BuildingManager> buildingManager;
    std::unique_ptr<CombatManager> combatManager;
    std::unique_ptr<InputHandler> inputHandler;
    
    bool spawnWanderer();
    void spawnInitialBeasts();
    void spawnStrongerBeasts();
    void renderGame();
    void loadNextLevel();
    std::vector<std::vector<char>> prepareFieldData() const;

public:
    GameController(int width = 15, int height = 15, bool loadFromSave = false);
    void runGame();
    
    Wanderer& getPlayer() { return wanderer; }
    GameField& getField() { return field; }
    BeastManager& getBeastManager() { return *beastManager; }
    BuildingManager& getBuildingManager() { return *buildingManager; }
    Position getPlayerPosition() const { return wandererPosition; }
    void setPlayerPosition(const Position& pos) { 
        field.getCell(wandererPosition.x, wandererPosition.y).setHasWanderer(false);
        wandererPosition = pos;
        field.getCell(pos.x, pos.y).setHasWanderer(true);
    }
    int getTurnCount() const { return turnCount; }
    void setTurnCount(int count) { turnCount = count; }
    int getCurrentLevel() const { return currentLevel; }
    void setCurrentLevel(int level) { currentLevel = level; }
    bool isLevelComplete() const;
    
    ~GameController();
};

#endif