#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "imagic_spell_card.h"

class Trap_spell: public IMagic_spell_card{
private:
    int damage;
    int cost;
    int radius;
    int type = 3;
public:
    Trap_spell(int damage = 8, int cost = 4, int radius = 8);
    int attack(Player& player, int target_x, int target_y, Place& place, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower);
    int getCost() const;
    int getRadius() const;
    int getType() const;
    std::string getName() const;
    std::string getDescription() const;
    void upgrade(int value) { damage, radius += value; };
};

#endif