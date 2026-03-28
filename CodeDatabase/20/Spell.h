#pragma once
#include <string>
#include "Gamefield.h"  

class Spell{
    private:

    std::string name;
    int damage;
    int range;

    public:
    
    std::string getName() const { return name; }
    int getDamage() const { return damage; }
    int getRange() const { return range; }

    void setName(const std::string& n) { name = n; }
    void setDamage(int d) { damage = d; }
    void setRange(int r) { range = r; }
    void virtual cast(std::string direction, Gamefield* gf,int pl_X, int pl_Y) = 0;






};