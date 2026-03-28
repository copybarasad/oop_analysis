#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Spell.h"

class SpellHand {
public:
    explicit SpellHand(std::size_t max_size);

    std::size_t GetMaxSize() const;

    std::size_t GetSize() const;

    bool IsFull() const;

    bool IsEmpty() const;

    bool AddSpell(std::unique_ptr<Spell> spell);

    std::string GetSpellName(std::size_t index) const;

    std::unique_ptr<Spell> TakeSpell(std::size_t index);

    void Clear() {
        spells_.clear();
    }

private:
    std::size_t max_size_;
    std::vector<std::unique_ptr<Spell> > spells_;

    void EnsureInvariants() const;
};
