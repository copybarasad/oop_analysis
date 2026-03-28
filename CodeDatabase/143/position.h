#ifndef POSITION_H
#define POSITION_H

#include <iostream>

class Position {
public:
    Position(int x = 0, int y = 0);

    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);
    void setPosition(int x, int y);

    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;

    void save(std::ostream& out) const;
    void load(std::istream& in);

private:
    int xCoordinate;
    int yCoordinate;
};

#endif



