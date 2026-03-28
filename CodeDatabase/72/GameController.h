#ifndef GAME_TERM_GAMECONTROLLER_H
#define GAME_TERM_GAMECONTROLLER_H

#include <memory>
#include "../commands/Command.h"
class Game;

template<typename InputHandlerType>
class GameController {
    std::unique_ptr<InputHandlerType> inputHandler;
    Game* game;
    bool isRunning;

public:

    explicit GameController(Game* game = nullptr, std::unique_ptr<InputHandlerType> handler = nullptr, const std::string& configFile = ""
        ) : game(game), isRunning(true) {
        if (handler) {
            inputHandler = std::move(handler);
        } else {
            inputHandler = std::make_unique<InputHandlerType>(configFile);
        }
    }

    explicit GameController(std::unique_ptr<InputHandlerType> handler)
        : game(nullptr), inputHandler(std::move(handler)), isRunning(true) {}

    std::unique_ptr<Command> getNextCommand() {
        if (!inputHandler || !isRunning) {
            return nullptr;
        }
        return inputHandler->getNextCommand();
    }


    bool isGameRunning() const {
        return isRunning;
    }

    void stop() {
        isRunning = false;
    }

};

#endif // GAME_TERM_GAMECONTROLLER_H