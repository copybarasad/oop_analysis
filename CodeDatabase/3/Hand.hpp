#pragma once
#include <vector>
#include <random>
#include "GameObjects/Spells/Spell.hpp"
#include "LevelManager/LMtypes.hpp"


class Hand {
public:
    Hand(HandDTO hand);
    HandDTO toDTO();
    
    void addRandomSpell();
    void deleteSpellByIndex(int index);
    Spell* getSpellByIndex(int index);
    void shakeHand();
    int size();

    void upTargetLvl();
    void upAreaLvl();
    void upTrapLvl();
private:
    int targetLvl;
    int areaLvl;
    int trapLvl;
    std::unique_ptr<Spell> createSpell(SpellType spell);
    std::vector<std::unique_ptr<Spell>> spells;
    int maxSize;
};
