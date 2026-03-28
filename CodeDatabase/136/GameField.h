#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>

struct Position {
    int x;
    int y;
    Position(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }

    bool operator<(const Position& other) const {
        return (x < other.x) || (x == other.x && y < other.y);
    }
};

enum class CellType {
    EMPTY,
    OBSTACLE,
    SLOW
};

class Cell {
    private:
        CellType type;
        bool hasWanderer;
        bool hasBeast;
        bool hasBuilding;
        bool hasTrap;
        int trapDamage;
    public:
        Cell();
        CellType getType() const;
        void setType(CellType newType);
        bool isEmpty() const;
        bool isPassable() const;
        bool getHasWanderer() const;
        void setHasWanderer(bool value);
        bool getHasBeast() const;
        void setHasBeast(bool value);
        bool getHasBuilding() const;
        void setHasBuilding(bool value);

        bool getHasTrap() const;
        void setHasTrap(bool value);
        int getTrapDamage() const;
        void setTrapDamage(int damage);
        void triggerTrap();
          
        void clear();
};

class GameField {
    private:
        int width;
        int height;
        std::vector<std::vector<Cell>> grid;

        void initializeField();
    public:
        GameField(int fieldWidth, int fieldHeight);
        
        //Конструктор копирования
        GameField(const GameField& other);
        //Конструктор перемещения
        GameField(GameField&& other) noexcept;
        //Операторы присваивания
        GameField& operator=(const GameField& other);
        GameField& operator=(GameField&& other) noexcept; 
        //Очищаем память, выделенную под vector
        ~GameField() = default;

        int getWidth() const;
        int getHeight() const;
        bool isValidPosition(int x, int y) const;
        Cell& getCell(int x, int y);
        const Cell& getCell(int x, int y) const;
        bool isCellPassable(int x, int y) const;
        bool findEmptyPosition(int& x, int& y) const;
        bool moveEntity(int fromX, int fromY, int toX, int toY, bool isWanderer);
        bool hasLineOfSight(int fromX, int fromY, int toX, int toY) const;
        void resize(int newWidth, int newHeight);
};

#endif