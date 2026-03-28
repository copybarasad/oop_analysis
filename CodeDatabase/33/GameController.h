#pragma once
#include "Command.h"
#include "Game.h"
#include "GameVisualizer.h"

template <typename InputReader, typename Renderer>
class GameController {
public:
    GameController(Game& game,
                   InputReader reader,
                   GameVisualizer<Renderer>& visualizer);

    Game::LevelResult RunLevel();

private:
    enum class CommandState { Invalid, SkipTurn, Consumed };
    CommandState HandlePlayerCommand(const Command& cmd);

private:
    Game& game_;
    InputReader reader_;
    GameVisualizer<Renderer>& visualizer_;
};
