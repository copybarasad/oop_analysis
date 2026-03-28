#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Game.hpp"
#include "commands.hpp"


template <typename RenderT>
class GameVisualizer {
private:
    RenderT renderer;
public:
    template <typename InputT>
    void draw(const Game& game, const InputT& input) {
        renderer.render(game, input);
    }
};

template <typename InputT>
class GameManager {
private:
    InputT inputHandler;
    Game* game; 

public:
    GameManager(Game* g, const std::string& configFile) : game(g) {
        inputHandler.loadConfig(configFile);
    }

    const InputT& getInputHandler() const {
        return inputHandler;
    }

    void processInput() {
        if (!game->isRunning()) return;
        
        Command* cmd = inputHandler.handleInput(*game);
        if (cmd) {
            cmd->execute(*game);
            delete cmd;
        }
    }
};

#endif