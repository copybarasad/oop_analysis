#pragma once
#include "Command.h"
#include "../include/PlayerCommand.h"

class GameCommand : public Command {
public:
    explicit GameCommand(PlayerCommand cmd) : cmd_(cmd) {}
    void execute(GameCycle& game) override {
        game.step(cmd_);
    }
private:
    PlayerCommand cmd_;
};
