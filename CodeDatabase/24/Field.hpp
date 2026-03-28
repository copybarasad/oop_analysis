#pragma once
#include "Player.hpp"
#include "Enemy.hpp"
#include "Pos.hpp"
#include "CellType.hpp"
#include "Configs.hpp"
#include "EnemyManager.hpp"
#include <vector>
#include <utility>
#include <memory>
#include <iostream>

// Класс игрового поля, отвечает только за состояние и координаты
class Field {
private:
    // Размеры поля
    int width, height;
    
    // Игрок и его позиция
    Player player;
    Pos playerPos;
    
    // Менеджер врагов
    std::unique_ptr<EnemyManager> enemyManager;
    
    // Игровая сетка
    std::vector<std::vector<CellType>> grid;

    // Инициализация игровой сетки
    void initializeGrid();
    
public:
    // Конструктор
    Field(int w = Config::Settings::DEFAULT_FIELD_WIDTH, 
          int h = Config::Settings::DEFAULT_FIELD_HEIGHT,
          const Pos& startPos = Pos(Config::Settings::PLAYER_START_X, 
                                  Config::Settings::PLAYER_START_Y));
    
    // Конструктор для создания поля по конфигурации уровня
    Field(const LevelConfig& config);
    
    // Конструктор копирования
    Field(const Field& other);
    
    // Конструктор перемещения
    Field(Field&& other) noexcept;
    
    // Оператор присваивания копированием
    Field& operator=(const Field& other);
    
    // Оператор присваивания перемещением
    Field& operator=(Field&& other) noexcept;
    
    // Перемещение объектов
    bool movePlayer(const Pos& direction);
    void moveEnemies();
    void addEnemy(std::unique_ptr<Enemy> enemy, const Pos& position);
    
    // Атака
    bool playerAttack(const Pos& direction);
    
    // Геттеры для доступа к состоянию
    int getWidth() const;
    int getHeight() const;
    CellType getCellType(const Pos& pos) const;
    Pos getPlayerPosition() const;
    const Player& getPlayer() const;
    Player& getPlayer();
    std::vector<std::pair<Enemy, Pos>> getEnemiesWithPositions() const;
    
    // Проверки состояния
    bool isValidPosition(const Pos& pos) const;
    bool isCellPassable(const Pos& pos) const;
    bool isEnemyAt(const Pos& pos) const;

    Enemy* getEnemyAt(const Pos& pos) const;
    void removeDeadEnemies();

    // Методы для загрузки игры
    void setPlayerPosition(const Pos& newPos);
    void setCellType(const Pos& pos, CellType type);
    void clearEnemies();
    void resizeGrid(int newWidth, int newHeight);
};
