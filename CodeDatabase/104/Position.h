#ifndef POSITION_H
#define POSITION_H

class Position {
private:
    int x_;
    int y_;

public:
    Position(int x, int y);
    int getX() const { return x_; }
    int getY() const { return y_; }
    void setX(int x) { x_ = x; }
    void setY(int y) { y_ = y; }
    double distanceTo(const Position& other) const;
    bool isWithinRange(const Position& other, int range) const;
};

#endif