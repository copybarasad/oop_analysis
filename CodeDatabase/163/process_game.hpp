#ifndef PROCESS_GAME_HPP
#define PROCESS_GAME_HPP

#include "game_controller.hpp"
#include "menu_controller.hpp"
#include <memory>

class ProcessGame {
private:
    std::unique_ptr<GameController> gameController;
    MenuController menuController;
    
    bool isGameActive;
    bool isPaused;
    
    char getCharacter() const;
    void clearScreen() const;
    void showPauseMenu();
    void processInGameMenu();
    
public:
    ProcessGame();
    
    void initializeNewGame();
    void startGameLoop();
    void pauseGame();
    void resumeGame();
    void endGame();
    
    bool loadGame(const std::string& filename = "savegame.dat");
    
    bool isRunning() const;
    bool isGamePaused() const;
};

#endif