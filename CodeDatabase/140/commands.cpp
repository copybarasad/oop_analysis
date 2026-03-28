#include "commands.h"
#include <iostream>
#include <limits>
#include <sstream>

void Command::display_help() const {
    renderer.render_help();
}

void Command::move_command(const string& direction) {
    int dx = 0, dy = 0;
    if (direction == "w") dy = -1;
    else if (direction == "s") dy = 1;
    else if (direction == "a") dx = -1;
    else if (direction == "d") dx = 1;
    
    if (field.is_player_slowed()) {
        cout << "You are slowed and cannot move this turn!\n";
        field.move_enemies();
        field.create_buildings();
        field.finish_turn();
    } else if (field.move_player(dx, dy)) {
        cout << "Moved.\n";
        field.move_enemies();
        field.create_buildings();
        field.finish_turn();
    } else {
        cout << "Cannot move there!\n";
    }
}

void Command::mode_switch() {
    field.switch_player_mode();
    cout << "Switching combat mode...\n";
    field.move_enemies();
    field.create_buildings();
    field.finish_turn();
}

void Command::attack(int x, int y) {
    field.player_attack(x, y);
    cout << "Attacked position (" << x << ", " << y << ").\n";
    field.move_enemies();
    field.create_buildings();
    field.finish_turn();
}

void Command::use_spell(std::size_t idx, int x, int y) {
    field.player_use_spell(idx, x, y);
    cout << "Spell used!\n";
    field.move_enemies();
    field.create_buildings();
    field.finish_turn();
}

void Command::save_game() {
    game.save_game();
}

void Command::load_game() {
    game.load_game();
}