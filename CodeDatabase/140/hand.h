#pragma once
#include <vector>
#include <memory>
#include "spell.h"

class Hand {
private:
    vector<unique_ptr<Spell>> spells;
    size_t max_size;
public:
    explicit Hand(size_t size = 3) : max_size(size) {}

    Hand(const Hand& other) : max_size(other.max_size) {
        for (const auto& s : other.spells) {
            spells.push_back(unique_ptr<Spell>(s->clone()));
        }
    }

    Hand& operator=(const Hand& other) {
        if (this != &other) {
            spells.clear();
            max_size = other.max_size;
            for (const auto& s : other.spells) {
                spells.push_back(unique_ptr<Spell>(s->clone()));
            }
        }
        return *this;
    }
    Hand(Hand&&) noexcept = default;
    Hand& operator=(Hand&&) noexcept = default;

    bool add_spell(unique_ptr<Spell> spell);
    void use_spell(size_t idx, Field& field, int caster_x, int caster_y, int target_x, int target_y);

    const vector<unique_ptr<Spell>>& get_spells() const;
    size_t size() const;
    bool is_full() const;
};