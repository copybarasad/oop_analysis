#ifndef POSITION_H
#define POSITION_H


#include "direction.h" 


// Класс для представления координат на игровом поле
// Инварианты: row >= 0, column >= 0
class Position {
public:
    Position(int row = 0, int column = 0);
    
    // Геттеры
    int getRow() const;
    int getColumn() const;
    
    // Проверка валидности позиции для поля заданного размера
    bool isValid(int maxRows, int maxColumns) const;
    
    // Получить соседнюю позицию в заданном направлении
    Position getNeighbor(Direction direction) const;
    
    // Операторы сравнения для удобства
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;

private:
    int row;
    int column;
};

#endif // POSITION_H