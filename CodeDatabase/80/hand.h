#ifndef HAND_H
#define HAND_H

#include "Spell.h"
#include <vector>
#include <memory>
#include <random>
#include <string>

class Hand {
private:
    std::vector<std::unique_ptr<Spell>> hand_buffer;
    int max_hand_spells;
    int num_spells;

public:
    Hand(int max_spells = 5);

    void Clear();
    bool AddRandomSpell();
    bool AddSpell(std::unique_ptr<Spell> spell);
    void Display();
    int CountSpells() const;
    void DisplaySpellInfo() const;
    std::unique_ptr<Spell> ChooseSpell(int index);
    void AddSpellForAchievement(int enemies_defeated);
    void UpgradeSpellDamage(int spellIndex, int damageIncrease);

    std::string GetSpellInfo(int index) const;

    std::string SaveToString() const;
    void LoadFromString(const std::string& data);
};

#endif 