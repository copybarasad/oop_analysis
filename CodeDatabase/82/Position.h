#ifndef POSITION_H
#define POSITION_H

class Position {
public:
    int x;
    int y;
    
    Position(int x = 0, int y = 0) : x(x), y(y) {}
    
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    
    Position operator+(const Position& other) const {
        return Position(x + other.x, y + other.y);
    }
    
    bool isValid(int maxX, int maxY) const {
        return x >= 0 && x < maxX && y >= 0 && y < maxY;
    }
};

#endif