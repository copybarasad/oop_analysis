#pragma once
#include <string>
#include <memory>
#include "Spell.h"
#include "Game.h"

class TrapSpell : public Spell {
public:
    TrapSpell(int damage);

    const std::string& Name() const override;
    bool Cast(Game& game, PlayerHand& hand) override;
    std::unique_ptr<Spell> Clone() const override;

private:
    int base_damage_;
    std::string name_;
};
