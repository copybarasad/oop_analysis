#pragma once
#include "Spell.h"
#include "Gamefield.h"
#include "Enemy.h"
#include "Cell.h"
#include <iostream>
#include <string>

class Lightning_bolt : public Spell {
private:
    int radius = 2;


public:
    //constructor
    Lightning_bolt();

    //functions
    void cast(std::string direction, Gamefield* gf,int pl_X, int pl_Y) override;
};