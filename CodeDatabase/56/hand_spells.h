#ifndef HAND_SPELL_H
#define HAND_SPELL_H

#include "Spell.h"
#include <vector>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>
#include "random.h"
class Hand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    size_t max_size;

public:
    Hand();
    
    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json& j);
    Spell& getSpell(int num);
    Hand(int size);
    void initilization_hand();
    void print_spells();
    void addSpell(std::unique_ptr<Spell> spell);
    void addRandomSpell();
    bool SearchSpellTrue(const std::string& name_spell);
    Spell* SearchSpell(const std::string& name_spell);
    void removeSpell(const std::string& name_spell);
    size_t getSpellCount();
    void UpSpell(int i);

    void clearHand();
};

#endif