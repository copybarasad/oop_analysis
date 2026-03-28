#pragma once
#include "Spell.h"
#include "Game.h"
#include "BuffState.h"
#include <string>

class AreaDamageSpell : public Spell {
public:
    AreaDamageSpell(std::string name, int damage, int areaSize);
    std::string GetName() const override;
    void ApplyBuffs(BuffState const& state) override;
    bool Apply(Player& caster, Game& game, Position target) override;

private:
    std::string name_;
    int damage_;
    int areaSizeX_;
    int areaSizeY_;
};