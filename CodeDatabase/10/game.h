#pragma once

#include "field.h"
#include "spell_basis.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include "player.h"
#include "enemy.h"
#include "tower.h"
#include "damage_tower.h"
#include "hand.h"
#include <vector>
#include <memory>
#include <stdexcept>
#include "save_system.h"

class Game {

private:
    Field* field;
    Player* player;
    std::vector<Enemy*> enemies;
    Tower* tower;
    DamageTower* damageTower;
    int currentLevel;
    bool gameRunning;
    int actionCounter;
    SaveSystem saveSystem;

    void initializeGame();
    void initializeLevel();
    void cleanup();
    
    void advanceToNextLevel();
    void handleGameOver();
    void removeHalfOfSpells();
    void upgradeEnemies();
    void upgradeTowers();
    void offerPlayerUpgrade();

public:
    Game();
    ~Game();

    void startNewGame();
    
    
    Field* getField() const;
    Player* getPlayer() const;
    const std::vector<Enemy*>& getEnemies() const;
    Tower* getTower() const;
    DamageTower* getDamageTower() const;
    int getCurrentLevel() const;
    bool isRunning() const;

    void setCurrentLevel(int level);
    void setGameRunning(bool running);
    void setField(Field* newField);
    void setPlayer(Player* newPlayer);
    void setTower(Tower* newTower);
    void setDamageTower(DamageTower* newTower);
    void clearEnemies();
    void addEnemy(Enemy* enemy);
    void updatePlayerEffects();

    void processEnemiesTurn();
    void processTowersTurn();
    void checkLevelCompletion();
    void checkGameOver();

    bool movePlayer(int dx, int dy);
    bool attack();
    bool changeCombatType();
    bool showStatus();
    bool showSpells();
    bool castSpell(int spellIndex, int targetX, int targetY);
    bool buySpell();
    bool saveGame();
    bool loadGame();
    bool quitGame();
    bool showMenu();
    bool restartGame();
};
