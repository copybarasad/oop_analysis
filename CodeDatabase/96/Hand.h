#pragma once
#include <vector>
#include <memory>
#include "Spell.h"
#include "struct/dataHand.h"



class Hand {
private:
    int maxSpell;
    std::vector<std::shared_ptr<Spell>> spells;
    int currentSpell;
public:
    Hand();
    Hand(dataHand data);
    ~Hand() = default;

    bool AddSpell(std::shared_ptr<Spell> spell);
    bool AddRandomSpell();
    bool UseSpell(SpellContext& ctx);
    void SellectSpell(int index);
    //const std::vector<std::shared_ptr<Spell>> GetSpells();
    const SpellType GetCurrentSpellType() const;
    dataHand Save();
    void ImproveAllSpells();
private:
    std::shared_ptr<Spell> GetCurrentSpell();

};