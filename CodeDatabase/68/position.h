#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <cmath>

struct Position {
    int x;
    int y;

    Position(int x = 0, int y = 0) : x(x), y(y) {}
    
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    } 

    double distanceTo(const Position& other) const {
        int dx = x - other.x;
        int dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

#endif
