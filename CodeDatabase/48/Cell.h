#ifndef CELL_H
#define CELL_H

class Cell {
private:
    bool isPassable;    // можно ли пройти через клетку
    bool hasPlayer;     // находится ли игрок на клетке
    bool hasEnemy;      // находится ли враг на клетке
    bool hasTower;      // находится ли башня на клетке
    char symbol;        // символ для отображения

public:
    Cell(bool passable = true);
    
    // Геттеры
    bool getIsPassable() const;
    bool getHasPlayer() const;
    bool getHasEnemy() const;
    bool getHasTower() const;
    char getSymbol() const;
    
    // Сеттеры
    void setIsPassable(bool passable);
    void setHasPlayer(bool hasPlayer);
    void setHasEnemy(bool hasEnemy);
    void setHasTower(bool hasTower);
    
    // Методы
    void updateSymbol();
    bool isEmpty() const;
};

#endif