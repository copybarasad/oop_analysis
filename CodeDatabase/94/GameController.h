#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "Spell.h"
#include "HealSpell.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include <memory>

enum class GameState {
    PLAYING,
    PLAYER_DIED,
    VICTORY
};

class GameController {
private:
    std::unique_ptr<GameField> field;
    Player* player;
    GameState currentState;
    int nextSpellThreshold;
    bool awaitingSpellTarget;
    int selectedSpellIndex;
    
    void handleEnemyAttacks();
    void handleEnemyMovement();
    void handleTowerAttacks();
    bool tryMoveEnemy(const Position& enemyPos);
    Position calculateEnemyMove(const Position& enemyPos, const Position& playerPos);
    void attackEnemyAt(const Position& pos);
    void giveRandomSpell();
    Position getSpellTargetFromDirection(int deltaX, int deltaY) const;

public:
    GameController(int fieldWidth, int fieldHeight);
    
    ~GameController();
    GameController(const GameController& other);
    GameController(GameController&& other) noexcept;
    GameController& operator=(const GameController& other);
    GameController& operator=(GameController&& other) noexcept;
    
    GameState getGameState() const { return currentState; }
    const GameField& getField() const { return *field; }
    GameField& getField() { return *field; }
    const Player& getPlayer() const { return *player; }
    Player& getPlayer() { return *player; }
    bool isAwaitingSpellTarget() const { return awaitingSpellTarget; }
    int getNextSpellThreshold() const { return nextSpellThreshold; }
    void setNextSpellThreshold(int threshold) { nextSpellThreshold = threshold; }
    
    // Публичный метод для добавления случайного заклинания
    void addRandomSpell();
    
    // Методы для управления врагами
    void clearEnemies();
    void addEnemy(const Position& pos, int health, int damage);
    std::vector<std::pair<Position, std::pair<int, int>>> getEnemiesInfo() const;
    
    // Методы для управления башней
    void clearTowers();
    void addTower(const Position& pos, int cooldown = 0);
    std::vector<std::pair<Position, int>> getTowersInfo() const;
    
    // Методы для получения состояния ловушек
    std::vector<Position> getTrapsInfo() const;
    
    bool movePlayer(int deltaX, int deltaY);
    bool switchCombatMode();
    bool useSpell(int spellIndex);
    bool castSpellAtTarget(int deltaX, int deltaY);
    void cancelSpellTarget();
    void update();
    void handleEnemyTurns();
};

#endif