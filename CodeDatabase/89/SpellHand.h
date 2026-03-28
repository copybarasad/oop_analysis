#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include <vector>
#include <memory>
#include <string>

class Spell;

class SpellHand {
public:
    SpellHand(int maxSize);
    ~SpellHand(); 

    bool addSpell(std::unique_ptr<Spell> spell);
    std::unique_ptr<Spell> useSpell(int index);
    size_t size() const;
    int getMaxSize() const { return maxSize; }

    void refillRandomSpell();
    std::string getSpellName(int index) const;

private:
    int maxSize;
    std::vector<std::unique_ptr<Spell>> spells;

    std::unique_ptr<Spell> createRandomSpell();
};

#endif