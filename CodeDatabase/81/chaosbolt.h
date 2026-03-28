#ifndef CHAOSBOLT_H
#define CHAOSBOLT_H

#include <random>
#include "Spell.h" 
#include "Cell.h"

class Chaosbolt: public Spell{
    int range = 8;
    int upgrades = 0;
    static const int less_damage = CHAOSBOLT_LESS_DAMAGE;
    static const int greater_damage = CHAOSBOLT_GREATER_DAMAGE;
    static const int tower_less_damage = TOWER_LESS_DAMAGE;
    static const int tower_greater_damage = TOWER_GREATER_DAMAGE;
    std::string name = "Снаряд хаоса";
    std::mt19937 gen;
public:
    Chaosbolt() : Spell(0, "Снаряд хаоса"), gen(std::random_device{}()) {} 
    Chaosbolt(int upd) : Spell(upd, "Снаряд хаоса"), gen(std::random_device{}()) {}
    int cast(std::string coords, Field *field, int *counter_of_enemies, int *counter_of_towers, Player *player) override;
    static int cast(Player *player, std::mt19937& gen);
};

#endif