#pragma once

#include "Spell.h"

class EnhancementSpell : public Spell {
public:
    EnhancementSpell() = default;

    SpellResult cast(GameController& game, const Position& target, EnhancementContext& enhancement) override;
    std::string getName() const override;
    std::string getDescription() const override;
    char getSymbol() const override;
    int getRange() const override;
    bool isEnhancement() const override { return true; }
    std::unique_ptr<Spell> clone() const override;
};







