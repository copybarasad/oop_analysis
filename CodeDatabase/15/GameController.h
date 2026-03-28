#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Game.h"
#include <memory>
#include <iostream>

template <typename InputReaderT, typename VisualizerT>
class GameController {
public:
    GameController(Game& game, InputReaderT& input_reader, VisualizerT& visualizer)
        : game_(game), input_reader_(input_reader), visualizer_(visualizer) {}

    void run() {
        visualizer_.showStartupScreen(input_reader_.getControlsDescription());

        game_.startGame();
        
        visualizer_.updateAndRender();
        
        while (game_.isRunning()) {
            auto command = input_reader_.readCommand();
            
            if (command) {
                try {
                    command->execute(game_);
                } catch (const std::exception& e) {
                    game_.notifyLog(std::string("Error executing command: ") + e.what());
                }
            }
            
            game_.checkGameStatus();
            visualizer_.updateAndRender();
        }
    }

private:
    Game& game_;
    InputReaderT& input_reader_;
    VisualizerT& visualizer_;
};

#endif