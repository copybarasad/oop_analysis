#ifndef SPELL_H
#define SPELL_H

#include <string>
#include "constants.h"

class Field; 
class Player;

class Spell{
protected:
    int range;
    int upgrades;
    int damage;
    std::string name;
public:
    Spell() : upgrades(0), name("") {}
    Spell(int upd, const std::string& spell_name) : upgrades(upd), name(spell_name) {}
    std::string getName() const { return name; }
    virtual ~Spell() = default;
    virtual int cast(std::string coords, Field *field, int *counter_of_enemies, int *counter_of_towers, Player *player) {
        return 0;
    }
};

#endif