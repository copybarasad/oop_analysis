#pragma once
struct Position{
    int x, y;
    Position(int x, int y): x(x), y(y){}
    bool operator==(const Position& other){return this->x == other.x && this->y == other.y; }
    bool operator!=(const Position &other) { return this->x != other.x || this->y != other.y; }
};
