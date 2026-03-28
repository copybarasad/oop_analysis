#pragma once
#include <string>
#include <memory>
#include "Spell.h"
#include "Game.h"

class BoostSpell : public Spell {
public:
    BoostSpell(int boost_value);

    const std::string& Name() const override;
    bool Cast(Game& game, PlayerHand& hand) override;
    std::unique_ptr<Spell> Clone() const override;

private:
    int boost_value_;
    std::string name_;
};
