#ifndef POSITION_H
#define POSITION_H

class Position {
private:
    int coordinateX;
    int coordinateY;
    
public:
    Position();
    Position(int x, int y);
    
    int getX() const;
    int getY() const;
    
    void setX(int newX);
    void setY(int newY);
    
    bool operator==(const Position& other) const;
};

#endif