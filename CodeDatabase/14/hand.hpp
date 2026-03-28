#ifndef HAND_H
#define HAND_H

#include <vector>

#include "spell.hpp"

class Hand{
    private:
        size_t max_size;
        std::vector<Spell*> spells;
    
    public:
        Hand(size_t size);
        ~Hand();

        bool addSpell(Spell* spell);
        void useSpell(size_t index);

        Spell* getSpell(size_t index) const;
        const std::vector<Spell*>& getSpells() const;
        size_t getSpellCount() const;
        bool isFull() const;
};

#endif