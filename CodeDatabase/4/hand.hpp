#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include "spell.hpp"

class Hand {
    std::vector<std::unique_ptr<Spell>> spells;
    size_t max_size;
public:
    Hand(size_t capacity);
    bool add_spell(std::unique_ptr<Spell> spell);
    void list_spells() const;
    std::unique_ptr<Spell> use_spell(size_t index);
    const std::unique_ptr<Spell>& get_spell_at_index(size_t index) const;
    size_t size() const;
    size_t capacity() const;
    bool increase_capacity(size_t increment);
};