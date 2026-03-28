#pragma once

class Cell {
private:
    int x;
    int y;
    bool isPassable;
    bool isSlowing;
    bool hasTrap;

public:
    // Конструкторы
    Cell();
    Cell(int x, int y, bool passable = true, bool slowing = false, bool hasTrap = false);

    // Геттеры
    bool getIsPassable() const;
    bool getIsSlowing() const;
    bool getHasTrap() const;

    // Сеттеры
    void setIsPassable(bool passable);
    void setIsSlowing(bool slowing);
    void setHasTrap(bool hastrap);

};