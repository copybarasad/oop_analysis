#pragma once

#include "cmath"

class Position
{
public:
    int x = -1, y = -1;
    Position() = default;
    Position(int x, int y) : x(x), y(y) {};

    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
    bool operator<(const Position& other) const;

    static int manhattan_distance(const Position &from, const Position &to);

    static int euclidean_sq_distance(const Position &from, const Position &to);
};

const Position NO_POSITION = {-1, -1};