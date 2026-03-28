#pragma once

#include <vector>
#include <memory>
#include "interfaces/ISpell.h"

class Field;
class Player;

class Hand {
    std::vector<std::unique_ptr<ISpell>> spells;
    int maxSize;
public:
    Hand(int maxSize = 4) : maxSize(maxSize) {
        addRandomSpell();
    }

    bool isFull() const { return spells.size() >= maxSize; }
    bool isEmpty() const { return spells.empty(); }
    int getSize() const { return spells.size(); }
    int getMaxSize() const { return maxSize; }
    ISpell* getSpell(int index) {
        if (index < 0 || index >= spells.size()) return nullptr;
        return spells[index].get();
    }
    const std::vector<std::unique_ptr<ISpell>>& getSpells() const { return spells; }
    bool isIndexInRange(int index) const {
        return index >= 0 && index < spells.size();
    }

    bool addSpell(std::unique_ptr<ISpell> spell);
    bool useSpell(int index, Field& field, Player* caster, int targetX, int targetY);
    void removeSpell(int index);
    void addRandomSpell();
};
