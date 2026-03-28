#include "BuffSpell.h"
#include "Game.h"
#include "Player.h"

#include <stdexcept>
#include <iostream>

BuffSpell::BuffSpell(int stacks, int cost)
    : stacks_(stacks),
      cost_(cost) {
    EnsureInvariants();
}

std::string BuffSpell::GetName() const {
    return "Buff";
}

int BuffSpell::GetCost() const {
    return cost_;
}

bool BuffSpell::CanCast(const Game & /*game*/,
                        std::size_t /*x*/,
                        std::size_t /*y*/) const {
    return true;
}

bool BuffSpell::Cast(Game &game,
                     std::size_t /*x*/,
                     std::size_t /*y*/) {
    Player &player = game.GetPlayer();
    player.AddBuffStack(stacks_);

    std::cout << "You feel power growing... (+" << stacks_
            << " buff stack(s), total "
            << player.GetPendingBuffStacks() << ")\n";
    return true;
}

void BuffSpell::EnsureInvariants() const {
    if (stacks_ <= 0) {
        throw std::logic_error("BuffSpell stacks must be > 0");
    }
    if (cost_ < 0) {
        throw std::logic_error("BuffSpell cost must be >= 0");
    }
}
