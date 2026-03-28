#ifndef POSITION_H
#define POSITION_H

struct Position
{
    int x;
    int y;

    Position(int xCoord = 0, int yCoord = 0);
    bool operator==(const Position &other) const;
    bool operator!=(const Position &other) const;
};

#endif