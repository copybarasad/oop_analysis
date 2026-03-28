#pragma once
#include <memory>
#include "spell.hpp"
#include "enums.hpp"

class SpellFactory {
public:
    std::unique_ptr<Spell> CreateSpell(SpellType type) const;
    std::unique_ptr<Spell> CreateRandomSpell() const;
    static SpellType FromString(const std::string& name);
};