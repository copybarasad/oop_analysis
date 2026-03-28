#pragma once
#include "GameEnt.h"


class Enemy:public GameEnt{

public:
    Enemy(double health = 10, double damage = 1,int pos_x = 0, int pos_y = 0);

    ~Enemy() = default;

    GameEnt* clone() const override {return new Enemy(*this);}

};