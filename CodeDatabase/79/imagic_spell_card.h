#ifndef IMAGIC_SPELL_CARD_H
#define IMAGIC_SPELL_CARD_H

#include <iostream>
#include <vector>

class Player;
class Place;
class Enemy;
class Tower;
class MagicTower;
class Tower;
class Player;

class IMagic_spell_card {
public:
    virtual int attack(Player& player, int target_x, int target_y, Place& place, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower) = 0;
    virtual int getCost() const = 0;
    virtual int getRadius() const = 0;
    virtual int getType() const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual ~IMagic_spell_card() = default;
};

#endif