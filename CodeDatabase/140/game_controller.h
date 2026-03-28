#pragma once

#include "game_command.h"
#include "game.h"
#include "commands.h"

class Game;
class Command;

template <typename TInput>
class GameController {
public:
    GameController(Game& game, Command& command, TInput inputProvider)
        : game(game),
          command(command),
          inputProvider(std::move(inputProvider))
    {}
    void step();
    void run_loop();
    void handle_command(const GameCommand& gc);

private:
    Game& game;
    Command& command;
    TInput inputProvider;
};
