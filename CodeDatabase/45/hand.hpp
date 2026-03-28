#ifndef HAND_HPP
#define HAND_HPP

#include "spell.hpp"
#include "map.hpp"
#include <vector>
#include <memory>
#include <string>

class Hand {
private:
    int capacity;
    std::vector<std::unique_ptr<Spell>> spells;
    
public:
    Hand(int capacity);
    void addSpell(std::unique_ptr<Spell> spell);
    void addRandomSpell();
    bool castSpell(int spellNumber, Map* map);
    void print_hand();
    void remove_half_spells();

    std::string handToString();
};

#endif