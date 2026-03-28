#pragma once
#ifndef POSITION_H
#define POSITION_H

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
};

#endif