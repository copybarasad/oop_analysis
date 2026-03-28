#ifndef POSITION_H
#define POSITION_H

struct Position {
    int x, y;
    Position(int x = 0, int y = 0) : x(x), y(y) {}
    
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }

     Position operator+(const Position& other) const {
        return Position(x + other.x, y + other.y);
    }
};

#endif