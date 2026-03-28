#pragma once
#include "Command.h"
#include "../include/PlayerCommand.h"
#include "../../entities/include/Player.h"

class SwitchModeCommand : public Command {
public:
    void execute(GameCycle& game) override {
        auto* p = dynamic_cast<Player*>(game.manager().entity(game.playerId()));
        if (p) {
            AttackMode newMode = (p->attackMode() == AttackMode::Melee) ? AttackMode::Ranged : AttackMode::Melee;
            PlayerCommand cmd;
            cmd.action = PlayerAction::SwitchMode;
            cmd.mode = newMode;
            game.step(cmd);
        }
    }
};
