#include "player_hand.h"
#include <iostream>

HandSpell::HandSpell(BookOfSpells& book, int maxSize) 
    : book(book), maxSize(maxSize) {
    addSpell(book.createSpellByName("Fireball"));
}

void HandSpell::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() < maxSize) {
        for (const auto& existingSpell : spells) {
            if (existingSpell->getName() == spell->getName()) {
                std::cout << "You already have " << spell->getName() << " in your hand!\n";
                return;
            }
        }
        spells.push_back(std::move(spell));
    } else {
        std::cout << "Hand is full! Cannot add more spells.\n";
    }
}

bool HandSpell::removeSpell(int index) {
    if (index >= 0 && index < spells.size()) {
        spells.erase(spells.begin() + index);
        return true;
    }
    return false;
}

Spell* HandSpell::getSpell(int index) const {
    if (index >= 0 && index < spells.size()) {
        return spells[index].get();
    }
    return nullptr;
}

Spell* HandSpell::getSpellForName(const std::string& name) const {
    for (const auto& spell : spells) {
        if (spell->getName() == name) {
            return spell.get();
        }
    }
    return nullptr;
}

void HandSpell::displayAllName() const {
    for (const auto& spell : spells) {
        std::cout << spell->getName() << " ";
    }
    std::cout << std::endl;
}