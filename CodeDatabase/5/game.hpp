#ifndef GAME_H
#define GAME_H

#include "save_manager.hpp"
#include <vector>
#include <memory>

class Level;

class Game {
private:
    std::vector<std::unique_ptr<Level>> levels; 
    int currentLevel;
    bool gameRunning;
    SaveManager saveManager;

public:
    Game();
    ~Game(); 
    void run();
    void startNewGame();
    bool loadGame();
    void saveGame();
    void showMainMenu();
    void showGameOverMenu(bool won);
    void setCurrentLevel(int level) { currentLevel = level; }
    void continueLoadedGame();
    bool hasSave() const { return saveManager.saveExists(); }
    
    // Геттеры
    int getCurrentLevel() const { return currentLevel; }
    const Level& getLevel(int index) const;
    Level& getLevel(int index);
    
private:
    void initializeLevels();
    bool runLevel(int levelIndex);
};

#endif