#pragma once
#include "GameCycle.h"
#include "../commands/Command.h"
#include "GameException.h"
#include <memory>
#include <iostream>

template <typename InputReaderT>
class GameController {
public:
    GameController(GameCycle& game) : game_(game) {
        inputReader_ = std::make_unique<InputReaderT>();
    }

    template <typename VisualizerT>
    void run(VisualizerT& visualizer) {
        while (game_.isRunning()) {
            visualizer.draw(game_);
            
            try {
                auto cmd = inputReader_->readCommand(game_);
                if (cmd) {
                    cmd->execute(game_);
                }
            } catch (const QuitGameException&) {
                break;
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
                std::cin.ignore(); std::cin.get();
            }
        }
    }

private:
    GameCycle& game_;
    std::unique_ptr<InputReaderT> inputReader_;
};
