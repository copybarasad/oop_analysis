#pragma once

#include "Core/Coordinates.h"

class MasterCall 
{
protected:
    Coordinates destination;
    int damage;
    int Points;
    int slowdown;
public:
    MasterCall(Coordinates destination, int damage = 0, int Points = 0, int slowdown = 0)
        :destination(destination),damage(damage),Points(Points),slowdown(slowdown){}
    Coordinates getDestination() const { return destination; }
    int getDamage() const { return damage; }
    int getPoints() const { return Points; }
    int getSlowdown() const { return slowdown; }
};