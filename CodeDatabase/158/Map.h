#pragma once

#include <vector>
#include <cstdlib>
#include <sstream>
#include "Cell.h"

class Map 
{
private:
    std::vector<std::vector<Cell>> pos;
    int X_size, Y_size;
public:
    Map(int x = 10, int y = 10);
    Map(const Map& other);
    Map& operator=(const Map& other);
    Map(Map&& other) noexcept;
    Map& operator=(Map&& other) noexcept;

    Cell getCell(int x, int y) const;
    Cell& getCell(int x, int y);
    bool iswithinlimits(int x, int y);
    void draw();
    int getXSize() const;
    int getYSize() const;
    
    std::string serialize() const;
    void deserialize(const std::string& data);
};