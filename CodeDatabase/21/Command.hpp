#pragma once
#include "../IGameController.hpp"
#include "../GameState.hpp"
#include "../KeyboardLayout.hpp"


class Command {
public:
    ~Command() = default;
    virtual void execute(
        IGameController& gameController,
        GameState& currentState,
        GameState& prevState
    ) = 0;
    virtual std::string getCommandName() = 0;
};
