#ifndef GAME_H
#define GAME_H

#include "Map.h"
#include "GameManager.h"
#include "Player.h"
#include "Tower.h"
#include "AttackTower.h"
#include "GameEventManager.h"
#include "GameVisualizer.h"
#include "ConsoleRenderer.h"
#include "SaveSystem.h"
#include "LevelManager.h"
#include <memory>

struct GameRenderData;
template<typename T> class GameVisualizer;
class ConsoleRenderer;

class Game {
private:
    Map& map;
    GameManager manag;
    Player hero;
    Tower tower;
    AttackTower attack_tower;
    SaveSystem saveSystem;
    LevelManager levelManager;
    std::unique_ptr<GameVisualizer<ConsoleRenderer>> visualizer;

    int counter;
    bool gameRunning;
   
    int currentLevel;


    void InitializeLevel();
    void CleanupGame();
    void RemoveHalfOfSpells();
    bool CheckWinCondition();
    bool CheckLoseCondition();

    void ShowLevelUpMenu();     
    void ApplyPlayerUpgrade(int choice);  

   
    void ProcessLevelUpChoice(const std::string& input);


public:
    Game(Map& gameMap);
    ~Game() = default;
    bool isLevelComplete() const { return attack_tower.heallpoints <= 0; }
    bool hasNextLevel() const { return currentLevel < 3; } 
    GameRenderData GetRenderData();
    void AdvanceToNextLevel();
    void InitializeGame();
    void StartNewGame();
    void DisplayGameState();
    void ProcessEnemiesTurn();
    void ProcessStructuresTurn();

    bool loadGame(const std::string& saveName);
    std::vector<std::string> getSaveList();
    void SaveGameMenu();
    void LoadGameMenu();

    void HandleGameEnd();
    bool inMainMenu;
    bool inLoadMenu;
    bool inSaveMenu;
    bool levelCompleted;
    Map& getMap() { return map; }
    Player& getHero() { return hero; }
    Player& getPlayer() { return hero; }  
    Tower& getTower() { return tower; }
    AttackTower& getAttackTower() { return attack_tower; }

    int GetCurrentLevel() const { return currentLevel; }
    int getCounter() const { return counter; }

    bool isGameRunning()  {
        return gameRunning && !CheckLoseCondition() && !CheckWinCondition();
    }
    bool isGameOver()  { return CheckLoseCondition(); }
    bool isVictory()  { return CheckWinCondition(); }
    void showLoadMenu();
    bool isInMenu() const { return inMainMenu; }
    bool isInLoadMenu() const { return inLoadMenu; }
    bool isInSaveMenu() const { return inSaveMenu; }

    void setInMainMenu(bool value) { inMainMenu = value; }
    void setInLoadMenu(bool value) { inLoadMenu = value; }
    void setInSaveMenu(bool value) { inSaveMenu = value; }

    void forceGameOver() { hero.heallpoints = 0; }
    void forceVictory() { attack_tower.heallpoints = 0; }
    std::string getGameStats();

    void showMainMenu();  

    void setGameRunning(bool running) { gameRunning = running; }
    bool isInMainMenu() const { return inMainMenu; }
    void returnToMainMenu();  
};

#endif 