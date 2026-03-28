#ifndef CONTROLLERS_GAMECONTROLLER_H
#define CONTROLLERS_GAMECONTROLLER_H

#include "../Input/IInputHandler.h"
#include "../Commands/ICommand.h"
#include "../Game.h"
#include <memory>

class Game;

template<typename T>
class GameController {
    private:
        T input_handler_;

    public:
        template<typename... Args>
        explicit GameController(Args&&... args);

        template<typename V>
        GameResult run(Game& game, V& visualizer);

        IInputHandler& getInputHandler();
};

template<typename T>
template<typename... Args>
GameController<T>::GameController(Args&&... args) : input_handler_(std::forward<Args>(args)...) {}

template<typename T>
template<typename V>
GameResult GameController<T>::run(Game& game, V& visualizer) {
    IGameRenderer* renderer_ptr = visualizer.getRenderer();
    return game.run(input_handler_, renderer_ptr);
}

template<typename T>
IInputHandler& GameController<T>::getInputHandler() {
    return input_handler_;
}

#endif