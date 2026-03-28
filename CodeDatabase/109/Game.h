#ifndef GAME_H
#define GAME_H

#include "GameMap.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include "SpellHand.h"
#include "Ally.h"
#include "GameLevel.h"
#include "GameSave.h"
#include "Position.h"
#include <vector>
#include <string>

class Game {
private:
    friend class GameSave;
    GameMap map;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<EnemyTower> towers;
    std::vector<Position> traps;
    std::vector<Ally> allies;
    bool running;
    int currentLevel;
    GameLevel currentGameLevel;

    bool hasSavedGame;
    bool gameLoaded;

    void init();
    void updateEnemies();
    void checkCollisions();
    void render();
    void spawnEnemies(int count);
    void checkWinCondition();
    void spawnTowers(int count); 
    void GameOver();

    void processSpellInput(char input);
    void applySpellEffects(Spell* spell, const Position& targetPos);
    void checkTraps();
    void addRandomSpellToPlayer();

    void updateAllies(); 
    bool summonAlly(const Position& pos);
    void applyPermanentEnhancement(Spell* spell);

    void handleEnemyDefeat(int enemyIndex);
    void handleTowerDestruction(int towerIndex);
    void giveReward(bool isTower);

    void initializeLevel();
    void playerTurn();
    void alliesTurn();
    void enemiesTurn();
    void towersTurn();
    bool checkLevelCompletion();
    void handleLevelTransition();
    void showLevelUpMenu();
    void applyMagicUpgrade();
    void applyStrengthUpgrade();
    void removeRandomSpells(int count);
    
    std::vector<std::string> getSaveFiles() const;
    void handleSaveLoadExceptions(const std::exception& e);
    GameSave gameSave; 

public:
    Game(int width = 15, int height = 15, int startLevel = 1);
    
    // Пустой метод run (реализация в Game.cpp)
    void run() {}
    
    void startNewGame();
    void processGameLoop();
    // Нужно сделать saveGame и loadGame публичными
    void saveGame();
    void loadGame();

    bool isRunning() const { return running; }
    void updateGameState();

    void showSpellInfo();
    void showSaveLoadMenu();

    // Геттеры - УБРАТЬ ДУБЛИКАТЫ!
    const GameMap& getMap() const { return map; }
    const Player& getPlayer() const { return player; }
    Player& getPlayer() { return player; }
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    const std::vector<EnemyTower>& getTowers() const { return towers; }
    const std::vector<Position>& getTraps() const { return traps; }
    const std::vector<Ally>& getAllies() const { return allies; }
    int getCurrentLevel() const { return currentLevel; }
    
    // Методы для GameSave
    GameSave& getGameSave() { return gameSave; }
    const GameSave& getGameSave() const { return gameSave; }
    
    // Методы для доступа к сохранениям - ТОЛЬКО ОДИН РАЗ!
    bool getHasSavedGame() const { return hasSavedGame; }
    void setHasSavedGame(bool saved) { hasSavedGame = saved; }
    
    // Сеттеры если нужны
    void setCurrentLevel(int level) { currentLevel = level; }

    // Методы для GameController
    void movePlayer(int dx, int dy);
    void playerAttack(AttackType type);
    void playerSwitchAttackType();
    void castSpell(int spellIndex);
    void quitGame() { running = false; }
};

#endif