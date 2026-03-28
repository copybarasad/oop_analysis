#pragma once

#include "InputHandler.hpp"
#include "Command.hpp"
#include <memory>

class GameWorld;
class GameContext;

template<typename InputHandlerType>
class GameController {
public:
    GameController(GameWorld& world, GameContext& context)
        : world_(world), context_(context), inputHandler_(std::make_unique<InputHandlerType>()) {}

    bool processPlayerTurn() {
        auto command = inputHandler_->getCommand();

        if (!command || command->getType() == CommandType::Invalid) {
            return false;
        }

        command->execute(world_, context_);
        return true;
    }

private:
    GameWorld& world_;
    GameContext& context_;
    std::unique_ptr<InputHandlerType> inputHandler_;
};
