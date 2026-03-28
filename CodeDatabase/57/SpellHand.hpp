#pragma once
#include <vector>
#include <memory>
#include <random>
#include "Absorption.hpp"
#include "Fireball.hpp"
#include "Trap.hpp"
#include "AriseKnights.hpp"
#include "Enhancement.hpp"
#include <iostream>
#include "./Systems/GameLogger.hpp"

class Hero;
class InterfaceSpell;
class Field;


class SpellHand {
private:
    std::vector<InterfaceSpell*> spells;
    std::vector<InterfaceSpell*> unindicted_spells;

    const float ICON_SIZE = 64.f;
    const float PADDING = 10.f;    

public:
    void addRandomSpell();
    SpellHand() {
        unindicted_spells.push_back(new FireballSpell);
        unindicted_spells.push_back(new AbsorptionSpell);
        unindicted_spells.push_back(new TrapSpell);
        unindicted_spells.push_back(new Arrise);
        unindicted_spells.push_back(new EnhancementSpell);
        addRandomSpell();
    }

    ~SpellHand() {
        for(auto& spell : spells) {
            if(spell) {
                delete spell;
            }
        }
        
    }
    


    InterfaceSpell* getSpellByIndex(int index, InterfaceGameLogger& logger);

    void printSpells(InterfaceGameLogger& logger);

    int getCount() { return spells.size(); }

    void draw(sf::RenderWindow& window);

    std::vector<InterfaceSpell*>& getVectorSpells() { return spells; }
    std::vector<InterfaceSpell*>& getUnindictedSpells() { return unindicted_spells; }
    void clearAllSpells();
    void setUnindictedSpellsByName(const std::vector<std::string>& newUnindictedSpells);
    void setSpellsByName(const std::vector<std::string>& newSpells);
    InterfaceSpell* createSpellByName(const std::string& name);
    void setSpells(const std::vector<InterfaceSpell*>& newSpells);
    void reset();

    void moveFirstHalfToUnindicted();
    void moveSpellToUnindicted(int index);
    void moveSpellToLearned(int index);
    void UpgradeEnhancementSpell(int new_level_spells = 1);
    int getLevelEnhancementSpell();
};
