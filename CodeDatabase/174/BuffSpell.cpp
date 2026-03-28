#include "BuffSpell.h"

BuffSpell::BuffSpell() {}

bool BuffSpell::cast(GameField& /*field*/, const Position& /*pos*/, Player& player, Game& /*game*/) {
    player.getHand().incrementBuffCount();
    std::cout << "Buff applied! Next spell will be stronger." << std::endl;
    return true;
}

std::string BuffSpell::getName() const {
    return "Empower";
}