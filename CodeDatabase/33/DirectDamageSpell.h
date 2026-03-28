#pragma once
#include <string>
#include <memory>
#include "Spell.h"
#include "Game.h"

class DirectDamageSpell : public Spell {
public:
    DirectDamageSpell(int damage, int range);

    const std::string& Name() const override;
    bool Cast(Game& game, PlayerHand& hand) override;
    std::unique_ptr<Spell> Clone() const override;

private:
    int base_damage_;
    int base_range_;
    std::string name_;
};
