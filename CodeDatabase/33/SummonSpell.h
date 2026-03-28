#pragma once
#include <string>
#include <memory>
#include "Spell.h"
#include "Game.h"

class SummonSpell : public Spell {
public:
    SummonSpell(int base_count, int ally_hp, int ally_damage);

    const std::string& Name() const override;
    bool Cast(Game& game, PlayerHand& hand) override;
    std::unique_ptr<Spell> Clone() const override;

private:
    int base_count_;
    int ally_hp_;
    int ally_damage_;
    std::string name_;
};
