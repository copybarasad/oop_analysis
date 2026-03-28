#ifndef GAME_H
#define GAME_H

#include "game_map.h"
#include "entity_manager.h"
#include "level_generator.h"
#include "save_game.h"
#include "game_command.h"
#include "console_input_handler.h"
#include <ctime>
#include "game_controller.h" 
#include "console_render.h"
#include "game_visualizer.h"  


template<typename InputHandler>
class GameController;

class Game {
private:
    GameMap map;
    EntityManager* entities;
    bool gameRunning;
    SaveGame saveSystem; 
    int level;
    int width;
    int height;
    bool levelCompleted;
    bool playerDead;

    GameController<ConsoleInputHandler>* controller;
    GameVisualizer<ConsoleRenderer> visualizer;

public:
    Game();
    ~Game();

    void runGameLoop();
    void playerMove(int dx, int dy);
    void playerToggleCombat();
    bool playerCastSpell(int spellIndex);
    void saveGame();
    void setGameRunning(bool running);
    void startNewGame();
    bool loadGame();
    void levelUpPlayer(int choice);
    void nextLevel();
    bool isGameRunning() const;
    bool isLevelCompleted() const;
    bool isPlayerDead() const;
    GameMap& getMap();
    EntityManager* getEntities();
    int getLevel() const;
    Position selectTargetForSpell(int range);
    bool initiateSpellCast();
    void showMainMenu();


    GameVisualizer<ConsoleRenderer>& getVisualizer() { return visualizer; }
};

#endif
