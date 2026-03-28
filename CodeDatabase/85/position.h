#ifndef POSITION_H
#define POSITION_H

class Position {
private:
    int x_coord;
    int y_coord;

public:
    Position(int x = 0, int y = 0);
    int get_x() const;
    int get_y() const;
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
    Position operator+(const Position& other) const;
};

#endif