#include "Hand.h"
#include "GameField.h"
#include <iostream>

Hand::Hand(int maxSize) : maxSize(maxSize) {}

void Hand::AddSpell(Spell* spell) {
    if (spells.size() < static_cast<size_t>(maxSize)) {
        spells.push_back(spell);
    } else {
        std::cout << "Hand is full, can't add a spell.\n";
    }
}

Spell* Hand::GetSpell(int index) const {
    if (index >= 0 && index < static_cast<int>(spells.size()))
        return spells[index];
    return nullptr;
}

void Hand::UseSpell(int index, Player& player, GameField& field, int x, int y) const {
    if (index < 0 || index >= static_cast<int>(spells.size())) {
        return;
    }
    spells[index]->Use(&player, &field, x, y);
}
int Hand::GetSize() const {
    return static_cast<int>(spells.size());
}

int Hand::GetMaxSize() const {
    return maxSize;
}
