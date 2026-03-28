#pragma once
#include "Command.hpp"


class ControlCommand : public Command {
private:
    char controlCommand;
public:
    ControlCommand(char controlCommand) : controlCommand(controlCommand) {};
    ~ControlCommand() {};
    void execute(
        IGameController& gameController,
        GameState& currentState,
        GameState& prevState
    ) override;
    std::string getCommandName() override {
        return "Control command: " + controlCommand;
    }
private:
    void mainMenuExecute(
        IGameController& gameController,
        GameState& currentState,
        GameState& prevState
    );
    void loadMenuExecute(
        IGameController& gameController,
        GameState& currentState,
        GameState& prevState
    );
    void inGameExecute(
        GameState& currentState
    );
    void levelUpMenuExecute(
        IGameController& gameController,
        GameState& currentState
    );
    void gameOverExecute(
        IGameController& gameController,
        GameState& currentState,
        GameState& prevState
    );
    void pauseMenuExecute(
        IGameController& gameController,
        GameState& currentState,
        GameState& prevState
    );
    void autorsMenuExecute(
        GameState& currentState
    );
};

