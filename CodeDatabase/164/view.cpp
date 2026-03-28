#include "view.h"

Game_view::Game_view() = default;
    
void Game_view::print_game_state(Field& field, Hero& hero, Hand& hand, unsigned int win_points) {
    renderer.print_field(field);
    renderer.print_hero(hero, win_points);
    renderer.print_hand(hand);
};
    
void Game_view::print_main_menu(Control& control) {
    std::vector<std::string> options = {
        std::string(1, control.start) + " - to start new Game",
        std::string(1, control.load) + " - to load last Game",
        std::string(1, control.exit) + " - to exit"
    };
    renderer.print_message("Would you start new Game or load last Game?");
    renderer.print_menu(options);
};
    
void Game_view::print_turn_menu(Control& control, bool canUseSpell) {
    std::vector<std::string> options = {
        std::string(1, control.exit) + " - to leave game",
        std::string(1, control.move) + " - to move Hero",
        std::string(1, control.change_attack) + " - to change attack type"
    };
        
    if(canUseSpell) {
        options.push_back(std::string(1, control.use_spell) + " - to use spell");
    }
        
    renderer.print_message("\nWhat do you want to do next?");
    renderer.print_menu(options);
};

void Game_view::print_new_exit(Control& control) {
    std::vector<std::string> options = {
        std::string(1, control.yes) + " - yes",
        std::string(1, control.exit) + " - exit",
    };
        
    renderer.print_message("Start new Game?");
    renderer.print_menu(options);
};

void Game_view::print_save_exit(Control& control) {
    std::vector<std::string> options = {
        std::string(1, control.save) + " - yes, save",
        std::string(1, control.exit) + " - no, exit",
    };
        
    renderer.print_message("Save Game?");
    renderer.print_menu(options);
};

void Game_view::print_message(const std::string& msg) {
    renderer.print_message(msg);
};