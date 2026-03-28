#ifndef AREA_SPELL_H
#define AREA_SPELL_H

#include "imagic_spell_card.h"

class area_spell: public IMagic_spell_card{
private:
    int damage;
    int cost;
    int radius;
    int attack_radius;
    int type = 2;
public:
    area_spell(int damage = 3, int cost = 8, int radius = 2, int attack_radius = 1);
    int attack(Player& player, int target_x, int target_y, Place& place, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower);
    int getCost() const;
    int getRadius() const;
    int getType() const;
    std::string getName() const;
    std::string getDescription() const;
    void upgrade(int value) { damage, radius += value; };
};

#endif