#pragma once
#include "spell.hpp"

class DirectDamageSpell : public Spell {
public:
    bool UseSpell(Game& game, const SpellParams& params) override;  
    std::string getName() const noexcept override;    
    SpellType getType() const noexcept { return SpellType::kDirectDamage; }
};