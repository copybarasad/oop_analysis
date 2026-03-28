#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "GameController.h"
#include <iostream>

template <typename InputReaderT, typename RendererT>
class GameEngine {
public:
    GameEngine(GameController& gc) : game(gc) {
        input.loadConfig("controls.cfg");
    }

    void run() {
        game.initialize();
        while (!game.isExitRequested()) {
            renderer.render(game);
            if (!game.isPlayerAlive()) {
                game.logEvent(LogMsg::GAME_OVER, std::to_string(game.getPlayer().getScore()));
                renderer.render(game);
                int choice = game.getInputInt("\nYou died.\n1. Restart\n2. Exit\n> ");
                
                if (choice == 1) {
                    game.restartGame();
                    continue; 
                } else {
                    break; 
                }
            }
            auto command = input.readCommand();
            if (command) {
                command->execute(game);
            }
        }
    }

private:
    GameController& game;
    InputReaderT input;     
    RendererT renderer; 
};

#endif