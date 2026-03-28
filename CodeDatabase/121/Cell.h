#ifndef CELL_H
#define CELL_H

#include "CellType.h"

class Cell {
private:
    CellType type;
    bool occupied;

public:
    // Конструкторы
    Cell();
    Cell(CellType cellType);
    
    // Конструктор копирования
    Cell(const Cell& other) = default;
    
    // Конструктор перемещения
    Cell(Cell&& other) noexcept = default;
    
    // Операторы присваивания
    Cell& operator=(const Cell& other) = default;
    Cell& operator=(Cell&& other) noexcept = default;
    
    // Деструктор
    ~Cell() = default;
    
    // Геттеры и сеттеры
    CellType getType() const { return type; }
    bool isOccupied() const { return occupied; }
    
    void setType(CellType newType) { type = newType; }
    void setOccupied(bool isOccupied) { occupied = isOccupied; }
    
    bool isPassable() const;
};

#endif