#pragma once

#include "Command.h"
#include "Game.h"
#include <thread>
#include <chrono>

template <typename TInputReader>
class GameController {
private:
    TInputReader& reader;
    Game& game;
    bool isRunning;

public:
    GameController(TInputReader& r, Game& g)
        : reader(r), game(g), isRunning(true) {
    }

    void process() {
        Command cmd = reader.poll();
        if (cmd.type == CommandType::None) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            return;
        }
        game.handleInput(cmd);
    }

    void handleQuit() {
        isRunning = false;
        game.stop();
    }

    bool active() const { return isRunning; }
};