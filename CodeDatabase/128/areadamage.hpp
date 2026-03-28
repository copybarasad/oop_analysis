#pragma once
#include "spell.hpp"

struct EnemyUnit;

class AreaDamageSpell : public Spell {
public:
    bool UseSpell(Game& game, const SpellParams& params) override;
    std::string getName() const noexcept override; 
    SpellType getType() const noexcept { return SpellType::kAreaDamage; }
};