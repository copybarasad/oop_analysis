#include "game_display.h"
#include <iostream>

Game_display::Game_display(const Game_controller& controller) 
    : game_controller(controller) {}

void Game_display::display_game_field() const {
    const Gamefield& field = game_controller.get_game_field();
    const Player& player = game_controller.get_player();
    const auto& enemies = game_controller.get_enemies();

    std::cout << "=== LEVEL " << game_controller.get_current_level() 
              << " === " << field.get_width() << "x" << field.get_height() 
              << " (Save: 'l', Quit: 'q', Help: 'h')\n";
    
    for (int y = 0; y < field.get_height(); ++y) {
        for (int x = 0; x < field.get_width(); ++x) {
            Position pos(x, y);
            char cell_char = '.';
            
            if (pos == player.get_position()) {
                cell_char = 'P';
            } else {
                bool enemy_here = false;
                for (const auto& enemy : enemies) {
                    if (enemy->get_position() == pos) {
                        cell_char = 'E';
                        enemy_here = true;
                        break;
                    }
                }
                if (!enemy_here) {
                    if (field.get_cell_type(pos) == Cell_type::OBSTACLE) {
                        cell_char = '#';
                    } else {
                        cell_char = '.';
                    }
                }
            }
            std::cout << cell_char << ' ';
        }
        std::cout << std::endl;
    }

    std::cout << "Health: " << player.get_health() << "/" << player.get_max_health() << std::endl;
    std::cout << "Damage: " << player.get_damage() << std::endl;
    std::cout << "Combat style: " << (player.get_combat_style() == Combat_style::CLOSE ? "CLOSE" : "RANGED") << std::endl;
    std::cout << "Position: (" << player.get_position().get_x() << ", " << player.get_position().get_y() << ")" << std::endl;
    std::cout << "Score: " << player.get_score() << std::endl;
    std::cout << "Level: " << game_controller.get_current_level() << std::endl;
    std::cout << "Enemies left: " << enemies.size() << std::endl;
    std::cout << "Spells: " << player.get_spell_hand().get_current_size() << "/" << player.get_spell_hand().get_max_size() << std::endl;
}

void Game_display::display_help() const {
    std::cout << "=== GAME COMMANDS ===\n";
    std::cout << "Movement:\n";
    std::cout << "  w - move up\n";
    std::cout << "  a - move left\n";
    std::cout << "  s - move down\n";
    std::cout << "  d - move right\n";
    std::cout << "\nCombat:\n";
    std::cout << "  c - switch to close combat\n";
    std::cout << "  r - switch to ranged combat\n";
    std::cout << "  f - ranged attack (ranged mode only)\n";
    
    const Player& player = game_controller.get_player();
    std::cout << "  1-" << player.get_spell_hand().get_current_size() 
              << " - cast spell (select then enter target x y)\n";
    std::cout << "\nSpells:\n";
    std::cout << "  g - try to get new spell\n";
    std::cout << "  v - view spells\n";
    std::cout << "\nGame:\n";
    std::cout << "  l - save game\n";
    std::cout << "  h - show this help\n";
    std::cout << "  q - quit game (with save option)\n";
    std::cout << "=====================\n";
}

void Game_display::display_game_over() const {
    const Player& player = game_controller.get_player();
    
    std::cout << "=== GAME OVER ===\n";
    if (!player.is_alive()) {
        std::cout << "You died in battle!\n";
    } else {
        std::cout << "Game completed!\n";
    }
    std::cout << "Final score: " << player.get_score() << std::endl;
    std::cout << "Final level: " << game_controller.get_current_level() << std::endl;
    std::cout << "Enemies defeated: " << (player.get_score() / 10) << std::endl;
    std::cout << "=================\n";
}