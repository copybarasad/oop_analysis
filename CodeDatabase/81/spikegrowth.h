#ifndef SPIKEGROWTH_H
#define SPIKEGROWTH_H

#include "Spell.h" 

class Spikegrowth: public Spell{
    int range = 10;
    int rad = 2;
    int upgrades = 0;
    std::string name = "Шипы";
public:
    Spikegrowth() : Spell(0, "Шипы") {}
    Spikegrowth(int upd) : Spell(upd, "Шипы") {}
    int cast(std::string coords, Field *field, int *counter_of_enemies, int *counter_of_towers, Player *player) override;
};

#endif