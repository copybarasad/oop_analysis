#pragma once
#include "Game.h"
#include "Command.h"

template <typename TRenderer>
class GameVisualizer {
private:
    TRenderer renderer;
public:
    void draw(Game& game) {
        renderer.render(game);
    }
};

template <typename TInputReader, typename TVisualizer>
class GameManager {
private:
    TInputReader input_reader;
    TVisualizer visualizer;

public:
    void run(Game& game) {
        game.start_new_game();

        while (game.is_running()) {
            visualizer.draw(game);

            Command* cmd = input_reader.get_command();
            if (cmd) {
                cmd->execute(game);
                delete cmd;
            }

            if (game.is_running()) {
                game.update_world();
            }
        }
    }
};
