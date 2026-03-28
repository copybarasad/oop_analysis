#ifndef WEAK_DEFAULT_SPELL_H
#define WEAK_DEFAULT_SPELL_H

#include "imagic_spell_card.h"

class weak_default_spell: public IMagic_spell_card{
private:
    int damage;
    int radius;
public:
    weak_default_spell(int damage = 2, int radius = 3);
    int attack(Player& player, int target_x, int target_y, Place& place, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower);
    int getCost() const;
    int getRadius() const;
    int getType() const;
    std::string getName() const;
    std::string getDescription() const;
};

#endif