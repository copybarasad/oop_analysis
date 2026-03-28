#include "game_loop.h"
#include <iostream>
#include <conio.h>
#include "initializer.h"

Game_loop::Game_loop(Game_controller& g, Game_display& d) 
    : game(g), display(d) {}

void Game_loop::run() {
    if (game.can_load_game()) {
        std::cout << "Found saved game. Load it? (y/n): ";
        char choice = _getch();
        std::cout << choice << std::endl;
        
        if (choice == 'y' || choice == 'Y') {
            if (game.load_game()) {
                std::cout << "Game loaded successfully!\n";
                std::cout << "Press any key to continue...\n";
                _getch();
            } else {
                std::cout << "Failed to load game.\n";
            }
        }
    }
    
    display.display_help();
    std::cout << "Press any key to start...\n";
    _getch();

    while (!game.is_game_finished()) {
        system("cls");
        display.display_game_field();
        
        if (game.can_advance_level()) {
            std::cout << "\n ALL ENEMIES DEFEATED! \n";
            std::cout << "Advancing to next level automatically...\n";
            game.advance_to_next_level();
            std::cout << "Press any key to continue...\n";
            _getch();
            continue;
        }
        
        std::cout << "Enter command: ";
        char command = _getch();
        std::cout << command << std::endl;

        Position current_pos = game.get_player().get_position();
        Position new_pos = current_pos;

        bool should_process_turn = true;

        switch (command) {
            case 'w': new_pos = Position(current_pos.get_x(), current_pos.get_y() - 1); break;
            case 's': new_pos = Position(current_pos.get_x(), current_pos.get_y() + 1); break;
            case 'a': new_pos = Position(current_pos.get_x() - 1, current_pos.get_y()); break;
            case 'd': new_pos = Position(current_pos.get_x() + 1, current_pos.get_y()); break;
            case 'c': 
                game.process_player_combat_style_switch(Combat_style::CLOSE);
                std::cout << "Switched to close combat.\n";
                break;
            case 'r': 
                game.process_player_combat_style_switch(Combat_style::RANGED);
                std::cout << "Switched to ranged combat.\n";
                break;
            case 'f': 
                game.process_player_ranged_attack();
                break;
            case 'g':
                std::cout << "Attempting to get new spell...\n";
                game.process_player_get_spell();
                break;
            case 'v': 
                game.get_player().display_spells();
                should_process_turn = false;
                break;
            case 'h': 
                display.display_help();
                should_process_turn = false;
                break;
            case 'l':
                try {
                    game.save_game();
                    std::cout << "Game saved successfully!\n";
                } catch (const std::exception& e) {
                    std::cout << "Save failed: " << e.what() << std::endl;
                }
                should_process_turn = false;
                break;
            case '1': case '2': case '3': case '4': case '5': {
                int spell_index = command - '1';
                
                if (spell_index < game.get_player().get_spell_hand().get_current_size()) {
                    std::cout << "Enter target coordinates (x y): ";
                    int x, y;
                    std::cin >> x >> y;
                    Position target(x, y);
                    game.process_player_spell_cast(spell_index, target);
                } else {
                    std::cout << "No spell in that slot!\n";
                    should_process_turn = false;
                }
                break;
            }
            case 'q': 
                {
                    std::cout << "Save before quitting? (y/n): ";
                    char save_choice = _getch();
                    std::cout << save_choice << std::endl;
                    if (save_choice == 'y' || save_choice == 'Y') {
                        try {
                            game.save_game();
                            std::cout << "Game saved successfully!\n";
                        } catch (const std::exception& e) {
                            std::cout << "Save failed: " << e.what() << std::endl;
                        }
                    }
                    std::cout << "Goodbye!\n";
                    return;
                }
            default:
                std::cout << "Invalid command!\n";
                should_process_turn = false;
                break;
        }

        if (command == 'w' || command == 'a' || command == 's' || command == 'd') {
            game.process_player_move(new_pos);
        }

        if (should_process_turn && !game.is_game_finished()) {
            game.process_enemy_turn();
        }

        if (!game.is_game_finished()) {
            std::cout << "Press any key to continue...\n";
            _getch();
        }
    }

    system("cls");
    display.display_game_over();
    
    std::cout << "\nPlay again? (y/n): ";
    char play_again = _getch();
    std::cout << play_again << std::endl;
    
    if (play_again == 'y' || play_again == 'Y') {
        Initializer init;
        int width = init.get_field_width();
        int height = init.get_field_height();
        int enemy_count = init.get_enemy_count();
        
        Game_controller new_game(height, width, enemy_count);
        Game_display new_display(new_game);
        Game_loop new_loop(new_game, new_display);
        
        new_loop.run();
    } else {
        std::cout << "Thanks for playing!\n";
    }
}