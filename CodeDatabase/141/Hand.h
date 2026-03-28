#ifndef HAND_H
#define HAND_H

#include "Spell.h"
#include <vector>

class Hand {
private:
    std::vector<Spell *> spells;
    int maxSize;

public:
    Hand(int maxSize);

    Hand(const Hand &other);

    Hand &operator=(const Hand &other);

    ~Hand();

    bool addSpell(Spell *spell);

    void removeSpell(int index);

    Spell *getSpell(int index) const;

    int getSize() const;

    int getMaxSize() const;

    bool isFull() const;

    bool isEmpty() const;

    void display() const;
};

#endif
