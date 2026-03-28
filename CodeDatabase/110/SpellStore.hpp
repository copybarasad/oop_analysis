#ifndef SPELL_STORE_H
#define SPELL_STORE_H

#include "../Spell/BaseSpell/Spell.hpp"

class SpellStore {
    private:
    std::vector<std::string> store;

    public:
    SpellStore (std::vector<std::string> spells);

    size_t getSpellNumber () const;
    void removeSpell (size_t index);
    std::string getSpell (size_t index) const;
};

#endif