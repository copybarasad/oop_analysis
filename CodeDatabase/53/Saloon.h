#ifndef SALOON_H
#define SALOON_H

#include "Character.h"
#include "Position.h"


class Saloon
{
private:
    Position position;
    int time_freeze;
public:
    Saloon(int x, int y);

    //Геттеры
    int getX() const;
    int getY() const;
    int getTime_freeze() const;

    // Сеттеры
    void setPosition(int newX, int newY);
    void setTime_freeze(int new_time);
    
};

#endif