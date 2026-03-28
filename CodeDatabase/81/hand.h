#ifndef HAND_H
#define HAND_H

#include <vector>
#include <memory>
#include <string>
#include <random>
#include <iostream>
#include "constants.h"
#include "Spell.h"

class Fireball; 
class Chaosbolt;
class Spikegrowth;
class Field;
class Player;

class Hand{
    int size = HAND_SIZE;
    std::vector<std::unique_ptr<Spell>> list;
    std::mt19937 gen;
public:
    Hand();
    int cast_from_hand(int number, std::string coords, Field *field, int *counter_of_enemies, int *counter_of_towers, Player *player);
    void add_spell(int upgrades);
    void print_hand() const;
    void remove_spells();
    void save(std::ofstream& ofs) const;
    void load(std::ifstream& ifs);
};

#endif