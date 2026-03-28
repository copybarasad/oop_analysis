#include "CastSpellCommand.h"
#include "../Game.h"
#include "../Player.h"
#include "../Hand.h"
#include <iostream>

CastSpellCommand::CastSpellCommand(int index, int x, int y) 
    : index_(index), x_(x), y_(y) {}

CommandResult CastSpellCommand::execute(Game& game) {
    Player* player = game.GetPlayer();
    if (!player) return {false, "No player."};

    if (index_ < 0 || static_cast<size_t>(index_) >= player->GetHand().Size()) {
        return {false, "Invalid spell index."};
    }

    bool used = player->GetHand().UseSpell(static_cast<size_t>(index_), *player, game, Position{x_, y_});
    if (used) {
        return {true, "Spell used."};
    } else {
        return {false, "Spell ineffective (not used)."};
    }
}


