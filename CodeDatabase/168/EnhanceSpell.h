#pragma once

#include "Spell.h"

class EnhanceSpell : public Spell {
public:
    EnhanceSpell()
        : Spell("Усиление магии", 0, 0) {}

    SpellResult cast(GameController& game, const Position& target) override;

    std::unique_ptr<Spell> clone() const override {
        auto spell = std::make_unique<EnhanceSpell>();
        spell->applyEnhancement(enhanceLevel);
        return spell;
    }

    std::string getDescription() const override {
        return "Усиливает следующее заклинание (накапливается)";
    }
};

