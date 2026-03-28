#pragma once
#ifndef CELL_H
#define CELL_H

class Cell {
private:
    bool hasPlayer;
    bool hasEnemy;

public:
    Cell();

    // Геттеры
    bool getHasPlayer() const;
    bool getHasEnemy() const;

    // Сеттеры
    void setHasPlayer(bool value);
    void setHasEnemy(bool value);

    // Проверка, свободна ли клетка
    bool isEmpty() const;

    // Очистка клетки
    void clear();
};

#endif