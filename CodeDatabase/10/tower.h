#pragma once
#include "field.h"
#include "enemy.h"
#include <string>
#include <vector>

class Tower {
private:
    std::string name;
    int x, y;
    Field& field;

public:
    Tower(std::string name, Field& field, int startX, int startY);
    ~Tower() = default;

    bool spawnEnemy(std::vector<Enemy*>& enemies);
    
    std::string getName() const;
    int getX() const;
    int getY() const;
};
