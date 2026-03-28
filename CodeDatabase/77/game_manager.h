#pragma once
#include "command.h" 
#include <iostream>
#include "Prints.h"
#include <stdexcept>

template <typename InputHandlerT>
class GameManager {
    InputHandlerT inputHandler; 

public:

    template <typename GameT, typename VisualizerT>
    void run(GameT& game, VisualizerT& visualizer) {

        visualizer.update(game);

        while (true) {

            GameCommand cmd = inputHandler.getCommand();

            if (cmd.type == CommandType::Quit) {
                break;
            }

            if (cmd.type == CommandType::Save) {
                try {
                    game.saveGame("savefile.txt");

                    Prints::print_game_saved();
                } catch (const std::runtime_error& e) {
                    Prints::print_save_error(e);
                }
                continue; 
            }

            if (cmd.type == CommandType::Load) {
                try {
                    game.loadGame("savefile.txt");
                    Prints::print_game_loaded();
                    visualizer.update(game); 
                } catch (const std::runtime_error& e) {
                    Prints::print_load_error(e);
                }
                continue;
            }

            bool turnMade = true; 

            if (cmd.type == CommandType::Move) {
                game.movePlayer(cmd.x, cmd.y);
            }
            else if (cmd.type == CommandType::Attack) {
                game.playerAttack();
            }
            else if (cmd.type == CommandType::SwitchMode) {
                game.switchPlayerMode();
            }
            else if (cmd.type == CommandType::CastSpell) {
                game.playerCastSpell(cmd.param);
            }
            else if (cmd.type == CommandType::BuySpell) {
                game.playerBuySpell();
            }
            else {

                Prints::print_incorrect_input_string();
                turnMade = false;
            }

            if (turnMade) {
                game.update(); 
            }

            visualizer.update(game);

            if (game.isGameOver()) {
                Prints::print_game_over_string();
                break;
            }
        }
    }
};