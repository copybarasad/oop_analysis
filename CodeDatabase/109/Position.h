#ifndef POSITION_H
#define POSITION_H

class Position {
private:
    int x;
    int y;

public:
    Position(int x = 0, int y = 0);
    
    int getX() const;
    int getY() const;
    
    void setX(int x);
    void setY(int y);
    void set(int x, int y);
    
    bool operator==(const Position& other) const;
    void move(int dx, int dy);
};

#endif