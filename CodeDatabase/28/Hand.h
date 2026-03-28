#pragma once
#include <memory>
#include <vector>
#include "Spell.h"

class Hand {
    std::vector<std::unique_ptr<Spell>> spells;
    size_t capacity;
public:
    explicit Hand(size_t cap);

    size_t size() const;
    size_t max_size() const;
    const std::vector<std::unique_ptr<Spell>>& get_spells() const;

    bool add_spell(std::unique_ptr<Spell> sp);
    bool add_random_spell();
    void remove_half_spells();
};