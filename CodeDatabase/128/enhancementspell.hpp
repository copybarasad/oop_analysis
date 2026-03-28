#pragma once
#include <iostream>
#include "spell.hpp"
#include "gamemodel.hpp"

class EnhancementSpell : public Spell {
public:
    bool UseSpell(Game& game, const SpellParams& params) override;
    std::string getName() const noexcept override; 
    SpellType getType() const noexcept { return SpellType::kEnhancement; }
};
