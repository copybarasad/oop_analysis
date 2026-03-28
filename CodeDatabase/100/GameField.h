#pragma once
#include <vector>
#include "Cell.h"
#include "../interface/ConsoleRenderer.h"

class GameField {
private:
    int width;
    int height;
    std::vector<std::vector<Cell>> grid;

public:
    // Конструктор
    GameField(int width, int height);

    // Правило пяти
    ~GameField();                                       // Деструктор
    GameField(const GameField& other);                  // Конструктор копирования
    GameField& operator=(const GameField& other);       // Оператор присваивания копированием
    GameField(GameField&& other) noexcept;              // Конструктор пермещения
    GameField& operator=(GameField&& other) noexcept;   // Оператор присваивания перемещением

    // Геттеры
    int getWidth() const;
    int getHeight() const;
    const Cell& getCell(int x, int y) const;
    bool isCellPassable(int x, int y) const;
    bool isCellSlowing(int x, int y) const;
    bool isCellTrap(int x, int y) const;

    // Установить ловушку
    void setTrap(int x, int y, bool hasTrap);
    void setSlowing(int x, int y, bool isSlowing);
    void setPassable(int x, int y, bool passable);

    // Генерация спец. клеток
    void generateWalls(int wallCount);
    void generateSlowing(int slowCount);
};