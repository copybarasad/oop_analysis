#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "Cell.h"
#include "Position.h"
#include <vector>
#include <utility>

class GameField {
private:
    std::vector<std::vector<Cell>> cells;
    int width;
    int height;

public:
    // Конструкторы
    GameField(int fieldWidth, int fieldHeight, const Position& playerStartPos);
    
    // Конструктор копирования
    GameField(const GameField& other);
    
    // Конструктор перемещения
    GameField(GameField&& other) noexcept;
    
    // Оператор присваивания с копированием
    GameField& operator=(const GameField& other);
    
    // Оператор присваивания с перемещением
    GameField& operator=(GameField&& other) noexcept;
    
    // Деструктор
    ~GameField() = default;
    
    // Геттеры
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    // Методы доступа к клеткам
    bool isPositionValid(const Position& position) const;
    Cell& getCell(const Position& position);
    const Cell& getCell(const Position& position) const;
    bool canMoveToPosition(const Position& position) const;
    bool placeObjectAtPosition(const Position& position);
    void removeObjectFromPosition(const Position& position);

private:
    void initializeBlockedCells(const Position& playerStartPos);
    void initializeSlowedCells(const Position& playerStartPos);
    
    // Вспомогательный метод для глубокого копирования
    void copyFrom(const GameField& other);
    
    // Вспомогательный метод для перемещения
    void moveFrom(GameField&& other) noexcept;
};

#endif