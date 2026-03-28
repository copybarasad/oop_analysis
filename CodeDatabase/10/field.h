#pragma once
#include <vector>
#include <string>
#include "cell.h"

class Player;
class Enemy;

class Field {
private:
    int width;
    int length;
    std::vector<std::vector<Cell>> grid;
    
    void generateObstacles(int obstaclePercentage);
    void setLength(int y);
    void setWidth(int x);
    void validateObstaclePercentage(int percentage);
public:
    Field(int x, int y);
    Field(int x, int y, int obstaclePercentage);
    Field(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;
    ~Field() = default;

    bool isValidPosition(int x, int y) const;
    bool isCellFree(int x, int y) const;
    int getWidth() const;
    int getLength() const;
    Cell& getCell(int x, int y);
    const Cell& getCell(int x, int y) const;
    
    bool placePlayer(int x, int y);
    bool placeEnemy(int x, int y);
};
