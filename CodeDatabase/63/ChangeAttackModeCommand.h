#ifndef RPG_CHANGEATTACKMODECOMMAND_H
#define RPG_CHANGEATTACKMODECOMMAND_H

#include "Command.h"
#include "../Game.h"

class ChangeAttackModeCommand : public Command {
    std::shared_ptr<Game>& game;
public:
    explicit ChangeAttackModeCommand(std::shared_ptr<Game>& game) : game(game) {};

    std::pair<bool, std::string> execute() override {
        auto& player = game->getPlayer();
        switch (player.getMode()) {
            case PlayerModes::MELEE:
            case PlayerModes::RANGE:
                player.setMode(player.getMode() == PlayerModes::MELEE ? PlayerModes::RANGE : PlayerModes::MELEE);
                return {true, ""};
            default:
                return {false, ""};
        }
    }
};

#endif