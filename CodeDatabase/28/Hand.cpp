#include "Hand.h"
#include <cstdlib>

#include "DirectAttackSpell.h"
#include "AoeAttackSpell.h"
#include "TrapSpell.h"

Hand::Hand(size_t cap) : capacity(cap) {}

bool Hand::add_spell(std::unique_ptr<Spell> spell) {
    if (spells.size() >= capacity) return false;
    spells.push_back(std::move(spell));
    return true;
}

bool Hand::add_random_spell() {
    if (spells.size() >= capacity) return false;
    int t = std::rand() % 3;
    if (t == 0) {
        return add_spell(std::make_unique<DirectAttack>("DirectSpell", 4, 15));
    } else if (t == 1) {
        return add_spell(std::make_unique<AOEAttack>("AOESpell", 5, 20));
    } else {
        return add_spell(std::make_unique<Trap>("TrapSpell", 3, 30));
    }
    return false;
}

void Hand::remove_half_spells() {
    if (spells.empty()) return;
    size_t count_to_remove = spells.size() / 2;

    for (size_t i = 0; i < count_to_remove; ++i) {
        int idx = std::rand() % spells.size();
        std::swap(spells[idx], spells.back());
        spells.pop_back();
    }
}


size_t Hand::size() const { return spells.size(); }
size_t Hand::max_size() const { return capacity; }

const std::vector<std::unique_ptr<Spell>>& Hand::get_spells() const { return spells; }
