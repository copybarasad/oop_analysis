#pragma once
#include <string>
#include <memory>
#include "Spell.h"
#include "Game.h"
#include "Coord.h"

class AreaDamageSpell : public Spell {
public:
    AreaDamageSpell(int damage, int base_size);

    const std::string& Name() const override;
    bool Cast(Game& game, PlayerHand& hand) override;
    std::unique_ptr<Spell> Clone() const override;

private:
    int damage_;
    int base_size_;
    std::string name_;
};
