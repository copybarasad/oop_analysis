#ifndef POSITION_H
#define POSITION_H

#include <fstream>

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
    void move(int deltaX, int deltaY);
    
    bool operator==(const Position& other) const;
    bool isValid(int maxWidth, int maxHeight) const;
    void save(std::ofstream& file) const;
    void load(std::ifstream& file);
};

#endif