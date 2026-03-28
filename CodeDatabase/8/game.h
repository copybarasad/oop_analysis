#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "field.h"
#include "player.h"
#include "enemy.h"
#include "help_funcs.h"
#include "hand.h"           
#include "directspell.h"    
#include "trap.h"           
#include "enemytower.h"
#include "console_renderer.h"
#include "command_mapper.h"
#include "color_vis.h"
#include "save_sys.h"
#include <vector>
#include <string>
#include <functional>

template<typename RendererType>
class GameVisualization;

class Game {
private:
    const int MAX_LEVELS = 3;
    Field* field;
    Player* player;
    Enemy* enemy;
    EnemyTower* tower;
    int currentLevel;
    bool gameRunning;
    SaveSystem saveSystem;
    std::function<void()> redrawCallback;
    CommandMapper* commandMapper;
    std::string configFile;

    void cleanup();
    template<typename RendererType>
    void initializeField(GameVisualization<RendererType>& visualization);
    template<typename RendererType>
    void initializePlayer(GameVisualization<RendererType>& visualization);
    void initializeEnemy();
    void initializeTower();
    template<typename InputProvider, typename RendererType>
    void RestartOrExit(GameVisualization<RendererType>& visualization);
    template<typename InputProvider, typename RendererType>
    void restartGame(GameVisualization<RendererType>& visualization);
    void prepareNextLevel();
    template<typename InputProvider, typename RendererType>
    void runGameLoop(GameVisualization<RendererType>& visualization); 
public:
    Game(const std::string& configPath = "controls.config.txt");
    ~Game();
    int getCurrentLevel() const { return currentLevel; }
    Field* getField() const { return field; }
    Player* getPlayer() const { return player; }
    Enemy* getEnemy() const { return enemy; }
    EnemyTower* getTower() const { return tower; }

    template<typename InputProvider, typename RendererType>
    void startGame(GameVisualization<RendererType>& visualization);
    template<typename InputProvider, typename RendererType>
    bool playLevel(GameVisualization<RendererType>& visualization);
    bool isGameRunning() const { return gameRunning; }
    template<typename RendererType>
    void saveGame(GameVisualization<RendererType>& visualization); 
    bool loadGame();
    void setCurrentLevel(int level) { currentLevel = level; }
    void setGameRunning(bool running) { gameRunning = running; }
    CommandMapper* getCommandMapper() const { return commandMapper; }
    void setCommandMapper(CommandMapper* mapper) { commandMapper = mapper; }
};
