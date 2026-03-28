#ifndef POSITION_H
#define POSITION_H

#include <string>
#include "../enums/Enums.h"

class Position {
    int x;
    int y;
    
public:
    Position(int x = 0, int y = 0);
    
    int getX() const;
    int getY() const;
    void setX(int xNew);
    void setY(int yNew);
    
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
    
    Position getNeighbour(Direction dir) const;
    double distanceTo(const Position& other) const;
    
    std::string toString() const;
    static Position fromString(const std::string& str);
};

#endif