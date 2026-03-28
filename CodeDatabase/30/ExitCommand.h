#pragma once
#include "Command.h"
#include "../include/GameException.h"

class ExitCommand : public Command {
public:
    void execute(GameCycle&) override {
        throw QuitGameException();
    }
};