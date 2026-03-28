#ifndef DEFAULT_SPELL_H
#define DEFAULT_SPELL_H

#include "imagic_spell_card.h"

class default_spell: public IMagic_spell_card{
private:
    int damage;
    int cost;
    int radius;
    int type = 1;
public:
    default_spell(int damage = 5, int cost = 3, int radius = 6);
    int attack(Player& player, int target_x, int target_y, Place& place, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower);
    int getCost() const;
    int getRadius() const;
    int getType() const;
    std::string getName() const;
    std::string getDescription() const;
    void upgrade(int value) { damage, radius += value; };
};

#endif