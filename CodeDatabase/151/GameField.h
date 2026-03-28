#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "Cell.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include "TrapOnField.h"
#include <vector>

class GameField {
private:
    int width;
    int height;
    std::vector<std::vector<Cell>> grid;
    std::vector<Enemy*> enemies;
    std::vector<EnemyTower> towers;
    std::vector<TrapOnField> traps;
    friend class LoadSaveGame;
    
    void initializeGrid();

public:
    Player* player; 
    bool isValidPosition(const Position& pos) const;
    // Конструкторы
    GameField(int width, int height);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    ~GameField();

    // Операторы присваивания
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;

    // Геттеры
    int getWidth() const;
    int getHeight() const;
    Cell& getCell(const Position& pos);
    const Cell& getCell(const Position& pos) const;
    Player* getPlayer() const;
    const std::vector<Enemy*>& getEnemies() const;

    // Управление объектами
    void placePlayer(int x, int y);
    void addEnemy(int x, int y);
    void addTower(const EnemyTower& tower);
    void addTrap(const TrapOnField& trap);
    void removeDeadEnemies();
    void removeDeactivatedTraps();

    // Перемещение
    bool movePlayer(int dx, int dy);
    void moveEnemies();
    void updateTowers();
    void checkTraps();

    // Система заклинаний
    void applyDamageInArea(const Position& center, int areaSize, int damage);
    void applyDamageToPosition(const Position& pos, int damage);
    bool isEnemyAtPosition(const Position& pos) const;
    bool isTowerAtPosition(const Position& pos) const;
    bool isEmptyForTrap(const Position& pos) const;

    // Отображение
    void display() const;

    // Проверка состояния игры
    bool isGameOver() const;

    const std::vector<EnemyTower>& getTowers() const;
    const std::vector<TrapOnField>& getTraps() const;

    bool isLevelCleared() const;

    void setPlayer(Player* p);
};

#endif