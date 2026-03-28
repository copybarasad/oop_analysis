#include "BoostSpell.h"
#include "Game.h"

BoostSpell::BoostSpell() = default;

void BoostSpell::apply(Game& game, int target_x, int target_y) {

    int boosts = game.getPlayer().consumeBoostCharges();
    game.getPlayer().addBoostCharge(boosts + 1);

    game.setLastMessage("Spell power is gathering... Next spell will be boosted!");
    game.endPlayerTurn();
}

std::unique_ptr<ISpell> BoostSpell::clone() const {
    return std::make_unique<BoostSpell>(*this);
}

std::string BoostSpell::getName() const { return "Empower"; }
std::string BoostSpell::getDescription() const {
    return "Boosts the next spell you cast. Stacks.";
}

int BoostSpell::getTypeId() const {
    return 4;
}