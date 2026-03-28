#pragma once

class Position {
private:
    int x_;
    int y_;

public:
    Position(int x, int y);
    
    int getX() const;
    int getY() const;
    
    Position translated(int dx, int dy) const;
    int deltaX(const Position& other) const;
    int deltaY(const Position& other) const;
    int manhattanDistance(const Position& other) const;
    
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
};