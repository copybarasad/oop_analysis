#pragma once
#include "Spell.h"
#include "Player.h"
#include "Game.h"
#include "BuffState.h"
#include <string>

class DamageSpell : public Spell {
public:
    DamageSpell(std::string name, int damage, int range);
    std::string GetName() const override;
    void ApplyBuffs(BuffState const& state) override;
    bool Apply(Player& caster, Game& game, Position target) override;

private:
    std::string name_;
    int damage_;
    int range_;
};