#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Command.h"
#include <functional>

template <typename InputReaderT>
class GameController {
public:
    explicit GameController(InputReaderT reader) : reader_(reader) {}

    Command nextCommand() {
        return reader_.readCommand();
    }

    template <typename CommandHandler, typename IsRunning, typename OnTurnEnd, typename OnDisplay>
    void runLoop(CommandHandler commandHandler, IsRunning isRunning, OnTurnEnd onTurnEnd, OnDisplay onDisplay) {
        while (isRunning()) {
            onDisplay();
            Command cmd = nextCommand();
            if (cmd.type == CommandType::INVALID || cmd.type == CommandType::NONE) {
                continue;
            }
            bool turnExecuted = commandHandler(cmd);
            if (!isRunning()) break;
            if (turnExecuted) {
                onTurnEnd();
            }
        }
    }

private:
    InputReaderT reader_;
};

#endif
