#include "hand.hpp"
#include <iostream>

Hand::Hand(size_t capacity) : max_size(capacity) {}

bool Hand::add_spell(std::unique_ptr<Spell> spell) {
    if (spells.size() >= max_size) return false;
    spells.push_back(std::move(spell));
    return true;
}

void Hand::list_spells() const {
    for (size_t i = 0; i < spells.size(); ++i) {
        std::cout << i << ": " << spells[i]->name() 
                  << " (cost: " << spells[i]->cost() << ")\n";
    }
}

std::unique_ptr<Spell> Hand::use_spell(size_t index) {
    if (index >= spells.size()) return nullptr;
    auto spell = std::move(spells[index]);
    spells.erase(spells.begin() + index);
    return spell;
}

const std::unique_ptr<Spell>& Hand::get_spell_at_index(size_t index) const {
    if (index >= spells.size()) {
        static std::unique_ptr<Spell> null_spell;
        return null_spell;
    }
    return spells[index];
}

size_t Hand::size() const { return spells.size(); }
size_t Hand::capacity() const { return max_size; }

bool Hand::increase_capacity(size_t increment) {
    max_size += increment;
    return true;
}