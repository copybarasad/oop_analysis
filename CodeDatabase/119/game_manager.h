#pragma once
#include "command.h" 
#include <iostream>
#include "Prints.h"
#include "fileOpenError.h"
#include "saveDataError.h"

template <typename InputHandlerT>
class GameManager {
    InputHandlerT inputHandler; 

public:

    template <typename GameT, typename VisualizerT>
    void run(GameT& game, VisualizerT& visualizer) {

        visualizer.update(game);
        bool running = true;

        while(running){

            GameCommand cmd = inputHandler.getCommand();
            bool turnMade = true;
            switch (cmd.type)
        { 
            case CommandType::Quit:{
                return;
            }   
            case CommandType::Save:{
                try {
                    game.saveGame("savefile.txt");

                    Prints::print_game_saved();
                } catch (const FileOpenError& e ) {
                    Prints::print_save_error(e);
                } catch (const SaveDataError& e){
                    Prints::print_save_error(e);
                }
                break; 
            }
        
            case CommandType::Load:{
                try {
                        game.loadGame("savefile.txt");
                        Prints::print_game_loaded();
                        visualizer.update(game); 
                    } catch (const FileOpenError& e) {
                        Prints::print_game_loaded();
                    } catch (const SaveDataError& e){
                        Prints::print_game_loaded();
                    }
                    break;
            }
            
            case CommandType::Move:{
                    game.movePlayer(cmd.x, cmd.y);
                    break;
            }
            case CommandType::Attack:{
                game.playerAttack();
                break;
            }
            case CommandType::SwitchMode:{
                game.switchPlayerMode();
                break;
            }
            case CommandType::CastSpell:{
                game.playerCastSpell(cmd.param);
                break;
            }
            case CommandType::BuySpell:{
                game.playerBuySpell();
                break;
            }
            default:
                Prints::print_incorrect_input_string();
                turnMade = false;
                break;
        }
            if (turnMade)
            {
                game.update();
            }
            visualizer.update(game);

            if (game.isGameOver()) {
                Prints::print_game_over_string();
                running = false;
            }  
        }   
    }
};