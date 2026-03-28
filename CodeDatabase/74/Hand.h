#ifndef HAND_H
#define HAND_H

#include "Types.h"
#include "Spell.h"
#include "Serialization.h"
#include <vector>
#include <memory>

class Hand {
private:
    int size;
    std::vector<std::shared_ptr<Spell>> spells;
public:
    Hand(int size);

    
    void addSpell(std::shared_ptr<Spell> spell);
    std::shared_ptr<Spell> getSpell(int index) const;
    std::shared_ptr<Spell> generateSpell();
    void useSpell(int index);
    int getCount() const;
    bool isOpen() const;

    void log() const;

    json serialize() const;
    void deserialize(const json& j);

    void removeHalfRandomSpells();
};

#endif // HAND_H