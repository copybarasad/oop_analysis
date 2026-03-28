#pragma once
#include "Serializable.h"


class Position {
public:
    int x;
    int y;

    Position(int x = 0, int y = 0);
    bool operator==(const Position& other) const;
    void saveTo(SaveFileRAII &out) const;
    void loadFrom(SaveFileRAII &in);
};