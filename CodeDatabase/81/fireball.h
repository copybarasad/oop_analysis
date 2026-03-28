#ifndef FIREBALL_H
#define FIREBALL_H

#include "Spell.h" 

class Fireball: public Spell{
    int range = 7;
    int rad = 2;
    int upgrades = 0;
    static const int damage = FIREBALL_DAMAGE;
    std::string name = "Огненный шар";
public:
    Fireball();
    Fireball(int upd);
    int cast(std::string coords, Field *field, int *counter_of_enemies, int *counter_of_towers, Player *player) override;
};

#endif