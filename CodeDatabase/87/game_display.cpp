#include "game_display.hpp"
#include "player.hpp"
#include "gamefield.hpp"

Game_display::Game_display(const Game_controller& controller): game_controller(controller){}

void Game_display::display_game_field() const{
    const Player& player = game_controller.get_player();
    const Gamefield& field = game_controller.get_game_field();
    std::cout << "\nGame field\n";
    std::cout << "  ";
    for(int x = 0; x < field.get_width(); x++){
        if(x <= 9) std::cout << x << " ";
        else std::cout << (x % 10) << " ";
    }
    std::cout << std::endl;
    for(int y = 0; y < field.get_height(); y++){
        if(y <= 9) std::cout << y << " ";
        else std::cout << (y % 10) << " ";
        for(int x = 0; x < field.get_width(); x++){
            Position current_position(x, y);
            char cell_char = '.';
            if(!field.is_position_accessible(current_position)) cell_char = '*';
            else if(current_position == player.get_position()) cell_char = 'P';
            else{
                bool enemy_found = false;
                for(auto& enemy: game_controller.get_enemies()){
                    if(enemy->get_position() == current_position){
                        cell_char = 'E';
                        enemy_found = true;
                        break;
                    }
                }
                if(!enemy_found){
                    for(auto& tower: game_controller.get_towers()){
                        if(tower.get_position() == current_position){
                            cell_char = 'T';
                            enemy_found = true;
                            break;
                        }
                    }
                }
                if(!enemy_found){
                    for(auto& trap: game_controller.get_traps()){
                        if(trap.pos == current_position){
                            cell_char = 'X';
                            enemy_found = true;
                            break;
                        }
                    }
                }
            }
            std::cout << cell_char << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\nPlayer statistics:\n";
    std::cout << "Level: " << game_controller.get_current_level() << "\n";
    std::cout << "Health: " << player.get_health() << "/" << player.get_max_health() << "\n";
    std::cout << "Damage: " << player.get_damage() << "\n";
    std::cout << "Combat Style: " << (player.get_combat_style() == Combat_style::CLOSE_COMBAT ? "Melee" : "Ranged") << "\n";
    std::cout << "Score: " << player.get_score() << "\n";
    std::cout << "Position: (" << player.get_position().get_x() << ", " << player.get_position().get_y() << ")\n";
    std::cout << "Enemies remaining: " << game_controller.get_enemies().size() << "\n";
    std::cout << "Spells in hand:\n";
    auto& spells = player.get_hand().get_spells();
    for(size_t i = 0; i < spells.size(); i++){
        std::cout << (i + 1) << ") " << spells[i]->get_name() << " (Damage: " << spells[i]->get_damage() << ", Radius: " << spells[i]->get_radius() << ")\n";
    }
    if(spells.empty()){
        std::cout << "No spells available.\n";
    }
}

void Game_display::display_help() const{
    std::cout << "\nControls:\n";
    std::cout << "W/A/S/D - Movement (Up/Left/Down/Right)\n";
    std::cout << "C - Switch to melee\n";
    std::cout << "R - Switch to ranged combat\n";
    std::cout << "F - Ranged attack (in ranged combat mode)\n";
    std::cout << "H - Show help\n";
    std::cout << "Q - Quit game\n";
    std::cout << "B - Buy a new random spell (20 points)\n";
    std::cout << "U - Save game\n";
    std::cout << "L - Load game\n";
    std::cout << "1/2/3 - Use spell from hand (if available)\n";
    std::cout << "Map symbols: P-Player, E-Enemy, T-Tower, X-Trap, *-Barricade, .-Empty\n";
}

void Game_display::display_game_over() const{
    const Player& player = game_controller.get_player();
    if(!player.is_alive()){
        std::cout << "\nYou lost!\n";
    }
    else{
        std::cout << "\nYou defeated all enemies!\n";
    }
    std::cout << "Final score: " << player.get_score() << " points\n";
}