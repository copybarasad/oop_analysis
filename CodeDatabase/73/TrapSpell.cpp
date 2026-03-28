#include "TrapSpell.h"
#include "Game.h"

TrapSpell::TrapSpell(int damage) : damage_(damage) {}

void TrapSpell::apply(Game& game, int target_x, int target_y) {
    int active_damage = damage_;
    int boosts = game.getPlayer().consumeBoostCharges();
    if (boosts > 0) {
        active_damage += boosts * 10;
    }

    if (game.canPlaceEntityAt(target_x, target_y)) {
        game.placeTrap(target_x, target_y, active_damage);
        std::string message = "A magical trap has been set!";
        if (boosts > 0) {
            message += " (Boosted damage!)";
        }
        game.setLastMessage(message);
        game.endPlayerTurn();
    } else {
        game.setLastMessage("Cannot place a trap there.");
    }
}

std::unique_ptr<ISpell> TrapSpell::clone() const {
    return std::make_unique<TrapSpell>(*this);
}

std::string TrapSpell::getName() const { return "Magic Trap"; }
std::string TrapSpell::getDescription() const {
    return "Places a trap that deals " + std::to_string(damage_) + " damage when an enemy steps on it.";
}
int& TrapSpell::getDamage() { return damage_; }

int TrapSpell::getTypeId() const {
    return 2;
}