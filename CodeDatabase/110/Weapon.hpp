#ifndef WEAPON_H
#define WEAPON_H

#include <iostream>
#include <random>
#include "Item.hpp"

enum class CombatMode {
    CLOSE,
    RANGED
};

class Weapon : public Item {
    private:
    CombatMode type;
    unsigned int maxDamage;
    unsigned int minDamage;
    double critChance;
    unsigned int range;
    unsigned int critBonus;

    public:
    Weapon (std::string name, CombatMode type, unsigned int maxDamageValue, unsigned int minDamageValue, double critChance, unsigned int rangeValue, double critBonus);

    void setType (CombatMode type);
    void setMaxDamage (unsigned int maxDamageValue);
    void setMinDamage (unsigned int minDamageValue);
    void setCritChance (double critChance);
    void setRange (unsigned int rangeValue);

    CombatMode getType () const;
    unsigned int getRange () const;
    unsigned int getMinDamage() const;
    unsigned int getMaxDamage() const;
    double getCritChance() const;
    double getCritBonus() const;

    unsigned int calculateDamage ();
};

#endif