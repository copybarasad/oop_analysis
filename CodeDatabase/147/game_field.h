#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "cell.h"
#include "position.h"
#include "player.h"
#include "enemy.h"
#include "tower.h"
#include <vector>
#include <stdexcept>

class GameField {
public:
    // Конструкторы
    GameField(int rows, int columns, unsigned int seed);
    explicit GameField(int size, unsigned int seed);
    unsigned int getSeed() const { return _seed; }  
    // Правило пяти
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;
    ~GameField() = default;

    // Размеры
    int getRowCount() const;
    int getColumnCount() const;

    // Доступ к клеткам (константный и неконстантный)
    const Cell& getCell(const Position& position) const;
    Cell& getCell(const Position& position);

    // Движение юнита (включая бой при столкновении)
    void moveUnit(Unit* unit, Direction direction);

    // Удаление юнита с поля (очистка occupant в соответствующей клетке)
    void removeUnit(Unit* unit);

    // Отрисовка
    void display() const;

private:
    unsigned int _seed = 0;
    std::vector<std::vector<Cell>> grid;
    void initializeGrid(int rows, int columns);
    void generateClosedCells(double closedCellPercentage = 0.15); // 15% blocked/freeze
    void copyFrom(const GameField& other);
};
#endif // GAMEFIELD_H
