#ifndef SPELLSHOP_H
#define SPELLSHOP_H

#include <vector>
#include <memory>

class SpellCard;

class SpellShop {
private:
    std::vector<std::shared_ptr<SpellCard>> availableSpells;

public:
    SpellShop();
    void displayShop() const;
    std::shared_ptr<SpellCard> buySpell(int index); 
    int getSpellCount() const;
};

#endif