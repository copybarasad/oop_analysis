#ifndef PIT_H
#define PIT_H

#include "Position.h"

class Pit
{
private:
    Position position;
public:
    Pit(int x, int y);

    //Геттеры
    int getX() const;
    int getY() const;

    // Сеттеры
    void setPosition(int newX, int newY);
    
};

#endif