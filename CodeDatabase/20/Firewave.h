#pragma once
#include "Spell.h"
#include "Gamefield.h"
#include "Enemy.h"
#include "Cell.h"
#include <iostream>
#include <string>


class Firewave : public Spell {
private:
    
    int radius = 2;
    int damage = 5;
    std::string name = "Firewave";

 public:
    //constructor
    Firewave();    
    //functions
    void cast(std::string direction, Gamefield* gf, int pl_X, int pl_Y) override;
};