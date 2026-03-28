#include "initializer.h"
#include "game_controller.h"
#include "game_display.h"
#include "game_loop.h"
#include "save_system.h"
#include <conio.h>
#include <iostream>
#include <stdexcept>

int main() {
    try {
        std::cout << "    Created by Udalov Vadim\n";
        
        Initializer init;
        
        int width = init.get_field_width();
        int height = init.get_field_height();
        int enemy_count = init.get_enemy_count();
        
        std::cout << "\nInitializing game with field " << width << "x" << height 
                  << " and " << enemy_count << " enemies...\n";
        
        Game_controller game(height, width, enemy_count);
        Game_display display(game);
        Game_loop loop(game, display);
        
        loop.run();
        
    } catch (const std::exception& e) {
        std::cerr << "\n!!! FATAL ERROR !!!\n";
        std::cerr << "Game crashed with error: " << e.what() << std::endl;
        std::cerr << "Please report this issue.\n";
        std::cout << "Press any key to exit...";
        _getch();
        return 1;
    } catch (...) {
        std::cerr << "\n!!! FATAL ERROR !!!\n";
        std::cerr << "Game crashed with unknown error.\n";
        std::cerr << "Please report this issue.\n";
        std::cout << "Press any key to exit...";
        _getch();
        return 1;
    }
    
    return 0;
}