#ifndef SPELL_HAND
#define SPELL_HAND

#include <algorithm>
#include <iostream>
#include <vector>

class SpellHand {
    private:
    std::vector<std::string> hand;
    size_t level;

    public:
    SpellHand (size_t handCapacity, const std::string spellName, size_t level=0);
    ~SpellHand () = default;

    bool addSpell (const std::string spellName);
    void removeSpell (const std::string spellName);
    void upgradeHand (size_t levels);
    void restoreHand ();

    const std::string& getSpellName (size_t index) const;
    size_t getHandLevel () const;
    size_t getSpellNumber () const;
    bool isFull () const;
    size_t getHandCapacity () const;
};

#endif