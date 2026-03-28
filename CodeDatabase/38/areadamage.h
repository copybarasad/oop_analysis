#pragma once

#include "spell.h"
#include "player.h"
#include "enemy.h"
#include "field.h"
#include <cmath>
class AreaDamage: public Spell{
    public:
        AreaDamage(){
            this->damage = 40;
            this->type = AREA;
        }
        bool doDamage(Player &p, std::vector<Enemy> &enemies, Field &f);
};
