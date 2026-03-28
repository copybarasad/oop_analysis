#ifndef RPG_CANCELCOMMAND_H
#define RPG_CANCELCOMMAND_H

#include "Command.h"
#include "../Game.h"
#include "../Player.h"

class CancelCommand : public Command {
    std::shared_ptr<Game>& game;
public:
    explicit CancelCommand(std::shared_ptr<Game>& game) : game(game) {}

    std::pair<bool, std::string> execute() override {
        auto& player = game->getPlayer();
        switch (player.getMode()) {
            case PlayerModes::SPELL:
                player.getSpellsHand().clearSelection();
            case PlayerModes::SHOP:
                player.setMode(player.getPreviousMode());
            default:
                return {false, ""};
        }
    }
};

#endif