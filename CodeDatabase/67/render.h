#ifndef RENDER_H
#define RENDER_H

#include "game.h"
#include <iostream>

class ConsoleRenderer {
public:
    void render(game& g) {
        std::system("clear");

        if(g.get_field()) {
             g.get_field()->print_field();
        }
        
       if (g.get_player()) { 
            std::cout << "HP: " << g.get_player()->get_health_points() 
          << " Kills: " << g.get_player()->get_enemies_killed() 
          << " Level: " << g.get_level()
          << " Turns: " << g.get_turn()
          << "\n";

        }
        const auto& log = g.get_log();
        for (const auto& msg : log) {
            std::cout << " > " << msg << "\n";
        }
    }
};

#endif
