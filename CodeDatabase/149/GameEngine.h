#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "GameField copy.h"
#include "Player.h"
#include <vector>
#include <memory>
#include <fstream>

class SaveManager;

class GameEngine {
private:
    GameField field;
    Player player;
    bool gameActive;

    friend class SaveManager;

public:
    GameEngine(int fieldWidth, int fieldHeight);
    bool processPlayerMovement(int deltaX, int deltaY);
    void switchPlayerCombatMode();
    int processEnemyTurns();
    void addEnemy(std::unique_ptr<Enemy> enemy, const Position& position);
    int getWidth() const;
    int getHeight() const;
    bool castSpell(int spellIndex, const Position& targetPosition);
    bool canBuySpell() const;
    void buyRandomSpell();
    bool isGameActive() const;
    int getPlayerHealth() const;
    int getPlayerScore() const;
    CombatMode getPlayerCombatMode() const;
    Position getPlayerPosition() const;
    int getAliveEnemyCount() const;
    bool isValidPosition(const Position& pos) const;
    bool setPlayerPosition(const Position& pos);
    CellType getCellType(const Position& pos) const;
    std::vector<Position> getEnemyPositions() const;
    std::vector<int> getEnemyHealths() const;
    std::vector<int> getEnemyDamages() const;
    bool isEnemyAlive(int index) const;
    bool isEnemyBuildingAt(const Position& pos) const;
    bool isEnemyTower(int index) const;
    int getEnemyAttackRange(int index) const;
    void damageEnemy(int index, int damage);
    void damageBuilding(const Position& pos, int damage);
    int getPlayerDamage() const;
    
    int getPlayerMana() const;
    int getPlayerMaxMana() const;
    const SpellHand& getPlayerSpellHand() const;
    std::string getSpellName(int index) const;
    int getSpellManaCost(int index) const;
    
    std::vector<Position> getTrapPositions() const;
    bool placeTrap(const Position& position, int damage);
    
    void setPlayerHealth(int health);
    void setPlayerScore(int score);
    void setPlayerMana(int mana);
    void setPlayerCombatMode(CombatMode mode);
    
    void saveGameState(std::ofstream& file) const;
    void loadGameState(std::ifstream& file);
    
    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;
};

#endif