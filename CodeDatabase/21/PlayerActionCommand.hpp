#pragma once
#include "Command.hpp"


class PlayerActionCommand : public Command {
private:
    char actionSymbol;
public:
    PlayerActionCommand(char actionSymbol) : actionSymbol(actionSymbol) {};
    ~PlayerActionCommand() {};
    void execute(
        IGameController& gameController,
        GameState& currentState,
        GameState& prevState
    ) override;
    std::string getCommandName() override {
        return "Action command: " + actionSymbol;
    }

};
