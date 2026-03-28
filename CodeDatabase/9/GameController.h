#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <SFML/Graphics.hpp>
#include "Command.h"
#include "Game.h"

template <typename InputReader>
class GameController {
public:
    explicit GameController(Game& game)
        : game_(game), input_reader_() {}

    void ProcessInput(sf::RenderWindow& window) {
        Command command;
        if (!input_reader_.PollCommand(window, command)) {
            return;
        }

        if (command.type == CommandType::SaveGame) {
            game_.SaveDefault();
            return;
        }

        if (command.type == CommandType::LoadGame) {
            game_.LoadDefault();
            game_.ApplyBoardSizeToWindow(window);
            return;
        }
        if (game_.ApplyCommand(command)) {
            game_.update();
        }
        bool acted = false;

        while (input_reader_.PollCommand(window, command)) {
            if (command.type == CommandType::None) {
                continue;
            }
            if (game_.ApplyCommand(command)) {
                acted = true;
            }
        }

        if (acted) {
            game_.update();
        }
    }

private:
    Game& game_;
    InputReader input_reader_;
};

#endif