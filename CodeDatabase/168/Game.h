#pragma once

#include "GameController.h"
#include "GameExceptions.h"
#include <memory>
#include <string>
#include <fstream>

enum class GameState {
    MENU,
    LEVEL_PROGRESS,
    LEVEL_COMPLETE,
    LEVEL_FAILED,
    UPGRADE_PHASE,
    GAME_OVER,
    GAME_WON
};

enum class UpgradeChoice {
    HEALTH,
    MELEE_DAMAGE,
    RANGED_DAMAGE,
    UPGRADE_SPELL,
    NONE
};

enum class SpellType : int {
    DIRECT_DAMAGE = 0,
    AREA_DAMAGE = 1,
    TRAP = 2,
    SUMMON = 3,
    ENHANCEMENT = 4
};

class Game {
private:
    std::unique_ptr<GameController> currentLevel;
    int currentLevelNumber;
    int fieldWidth;
    int fieldHeight;
    GameState state;
    
    int playerMaxHealth;
    int playerMeleeDamage;
    int playerRangedDamage;
    
    static constexpr const char* SAVE_FILE = "savegame.dat";
    static constexpr const char* SAVE_MAGIC = "GAME_SAVE_V1";
    
    void initializeLevel(int levelNumber);
    void spawnLevelEnemies(int levelNumber);
    
    void saveGameData(std::ofstream& file) const;
    void loadGameData(std::ifstream& file);
    void saveSpell(std::ofstream& file, const Spell* spell) const;
    std::unique_ptr<Spell> loadSpell(std::ifstream& file);
    
    void showUpgradeMenu();
    UpgradeChoice getUpgradeChoice();
    void applyPlayerUpgrade(UpgradeChoice choice);
    void upgradeSelectedSpell();
    void removeHalfSpells();
    
    int calculateEnemyHealth(int level) const;
    int calculateEnemyDamage(int level) const;
    int calculateBuildingHealth(int level) const;
    int calculateTowerHealth(int level) const;
    int calculateTowerDamage(int level) const;
    
public:
    Game();
    ~Game() = default;
    
    void run();
    void startGame();
    void startNewGame();
    bool loadGame();
    void saveGame();
    
    void processPlayerTurn(char command);
    void processAlliesTurn();
    void processEnemiesTurn();
    void processBuildingsAndTowersTurn();
    void checkLevelConditions();
    
    void completeLevel();
    void failLevel();
    bool askRestartOrExit();
    
    GameState getState() const { return state; }
    GameController* getCurrentLevel() { return currentLevel.get(); }
    const GameController* getCurrentLevel() const { return currentLevel.get(); }
    int getCurrentLevelNumber() const { return currentLevelNumber; }
};

