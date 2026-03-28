#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <vector>
#include <string>
#include <memory>
#include "Player.h"
#include "GameLevel.h"
#include "EventLogger.h" 

class Field;

class GameController {
public:

    GameController(Player& player, Field& field, EventLogger& logger);
    
    void initialize();
    void restartGame();
    
    void handlePlayerMove(int dx, int dy);
    void handleSwitchMode();
    void handleCastSpell();
    void handleBuySpell();
    void handleSave();
    void handleLoad();
    void handleExit();
    
    bool isExitRequested() const;
    bool isPlayerAlive() const;
    int getCurrentLevel() const;
    
    const Field& getField() const;
    const Player& getPlayer() const;
    GameLevel* getLevel();
    
    void logEvent(LogMsg msg);
    void logEvent(LogMsg msg, int value);
    void logEvent(LogMsg msg, const std::string& text);

    int getInputInt(const std::string& prompt);
    char getInputChar(const std::string& prompt);

    void damageAt(int x, int y, int damage);
    void placeTrap(int x, int y, int damage);
    bool spawnAllyNear(int x, int y);
    void damagePlayer(int amount);

    void addEnemy(const Enemy& enemy);
    void addSpawner(const EnemySpawner& spawner);
    void addTower(const EnemyTower& tower);

private:
    void processTurn();
    void checkLevelCompletion();
    void startNextLevel();
    void levelUpMenu();
    
    void performSave(const std::string& filename);
    void performLoad(const std::string& filename);

    Player& player;
    Field& initialFieldRef; 
    std::unique_ptr<GameLevel> currentLevelObj;
    
    int currentLevelNumber;
    bool exitRequested;
    
    EventLogger& eventLogger; 
};

#endif