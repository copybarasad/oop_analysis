#pragma once

#include "game.hpp"
#include "command_manager.hpp"
#include "i_renderer.hpp"
#include "keybindings_manager.hpp"
#include <memory>

template<typename InputHandler>
class GameController{
private:
    Game* game;
    InputHandler* input_handler;
public:
    GameController(Game* game) : game(game), input_handler(new InputHandler()) {};
    ~GameController() {delete input_handler;};

    template<typename Visualizer>
    int process_turn(Visualizer& visualizer){
        std::string input = input_handler->read_input();
        CommandManager cmd = input_handler->parse_input(input);
        KeybindingsManager tmp_manager;

        switch (cmd.get_type()){
            case CommandType::HELP:
                visualizer.render_help(tmp_manager.get_keybindings());
                return 1;
            case CommandType::KEYBIND_SETUP:
                input_handler->setup_keybindings_interactive(visualizer.get_renderer());
                return 1;
            case CommandType::SHOW_CARDS:
                visualizer.render_player_hand(game->get_world()->get_player());
                return 1;
            default:
                return game->handle_command(cmd);
        }
        return 1;
    }

    template<typename Visualizer>
    void setup_keybindings_interactive(Visualizer& visualizer){
        input_handler->setup_keybindings_interactive(visualizer.get_renderer());
    }

    std::string get_input(std::string msg){
        return input_handler->get_input(msg);
    }
};