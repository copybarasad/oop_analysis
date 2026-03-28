#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "game.h"

template <typename InputReader>
class GameController {
private:
    InputReader reader;
    game& game_instance;

public:
    GameController(game& g) : game_instance(g) {}

        void update() {
            auto command = reader.readCommand(game_instance);
            if (command) {
                command->execute(game_instance);
                if (command->passesTurn()) {
                    game_instance.enemy_turn();
                    game_instance.tower_turn();                
                    game_instance.next_turn(); 
                }
            }
        }
};

#endif
