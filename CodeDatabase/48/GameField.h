#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "Cell.h"
#include <vector>
#include <memory>

class GameField {
private:
    std::vector<std::vector<Cell>> field;
    int width;
    int height;

public:
    // Конструкторы
    GameField(int w = 15, int h = 15);
    
    // Конструктор копирования
    GameField(const GameField& other);
    
    // Конструктор перемещения
    GameField(GameField&& other) noexcept;
    
    // Оператор присваивания с копированием
    GameField& operator=(const GameField& other);
    
    // Оператор присваивания с перемещением
    GameField& operator=(GameField&& other) noexcept;
    
    // Геттеры
    int getWidth() const;
    int getHeight() const;
    Cell& getCell(int x, int y);
    const Cell& getCell(int x, int y) const;
    
    // Проверка координат
    bool isValidPosition(int x, int y) const;
    
    // Методы работы с полем
    void clearCell(int x, int y);
    void setPlayerPosition(int x, int y);
    void setEnemyPosition(int x, int y);
    void setTowerPosition(int x, int y);
    void clearTowerPosition(int x, int y);
    bool isCellPassable(int x, int y) const;
    bool isCellEmpty(int x, int y) const;
    
    // Отображение поля
    void display() const;
    
    // Генерация препятствий
    void generateObstacles(double obstaclePercentage = 0.2); // 20% по умолчанию
    
private:
    // Вспомогательные методы
    void initializeField();
    void copyField(const GameField& other);
};

#endif