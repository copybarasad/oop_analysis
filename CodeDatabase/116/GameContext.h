#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "GameState.h"
#include "../systems/GameControl.h"
#include "../systems/InputHandler.h"
#include "../systems/StatsRenderer.h"
#include "../systems/FieldRenderer.h"
#include "SaveLoadManager.h"
#include "LevelManager.h"

class GameState;

class GameContext {
private:
    std::unique_ptr<GameState> currentState;
    
    std::unique_ptr<GameControl> controller;
    InputHandler inputHandler;
    StatsRenderer statsRenderer;
    FieldRenderer fieldRenderer;
    SaveLoadManager saveLoadManager;

    LevelManager levelManager;
    bool isRunning;
    bool gameRunning;
    int currentLevel;
    
public:
    GameContext();
    ~GameContext() = default;
    
    void transitionTo(const std::string& stateName);
    void requestQuit() { isRunning = false; }
    
    GameControl& getController() { return *controller; }
    InputHandler& getInputHandler() { return inputHandler; }
    StatsRenderer& getStatsRenderer() { return statsRenderer; }
    FieldRenderer& getFieldRenderer() { return fieldRenderer; }
    LevelManager& getLevelManager() { return levelManager; }
    SaveLoadManager& getSaveLoadManager() { return saveLoadManager; }
    
    void initializeNewLevel(bool restartGame = false);
    void completeCurrentLevel();
    
    bool isAppRunning() const { return isRunning; }
    bool isGameRunning() const { return gameRunning; }
    void setGameRunning(bool running) { gameRunning = running; }
    
    void handleInput();
    void render();
};