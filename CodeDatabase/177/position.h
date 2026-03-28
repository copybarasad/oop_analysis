#ifndef POSITION_H
#define POSITION_H

class Position {
private:
    int x;
    int y;

public:
    Position(int xCoord = 0, int yCoord = 0);
    
    int getX() const;
    int getY() const;
    void setX(int newX);
    void setY(int newY);
    
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
    Position operator+(const Position& other) const;
};

#endif
