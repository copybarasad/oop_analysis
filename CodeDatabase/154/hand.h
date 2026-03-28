#ifndef HAND_H
#define HAND_H

#include <vector>
#include "ispell.h"

class Field;

class Hand {
public:
    Hand(size_t maxSize);
    ~Hand();
    bool addSpell(ISpell* spell);
    bool playSpell(size_t index, int casterId, Field& field, int targetRow, int targetCol);
    void drawSpell(ISpell* spell);
    size_t size() const;
    size_t capacity() const;
    std::string getSpellName(size_t index) const;
    void addRandomSpell();

private:
    size_t maxSize_;
    std::vector<ISpell*> spells_;
};

#endif
