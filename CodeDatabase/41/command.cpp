#include <iostream>
#include "command.h"
#include "game.h"
#include "game_field.h"
#include "player.h"

Move_command::Move_command(int delta_x, int delta_y) : dx(delta_x), dy(delta_y) {}

void Move_command::execute(Game& game) {
    game.handle_move(dx, dy);
}

std::unique_ptr<Command> Move_command::clone() const {
    return std::make_unique<Move_command>(dx, dy);
}

std::string Move_command::get_name() const {
    if (dx == 0 && dy == -1) return "move_up";
    if (dx == 0 && dy == 1) return "move_down";
    if (dx == -1 && dy == 0) return "move_left";
    if (dx == 1 && dy == 0) return "move_right";
    return "move";
}

int Move_command::get_dx() const { return dx; }
int Move_command::get_dy() const { return dy; }

void Attack_command::execute(Game& game) {
    game.handle_attack();
}

std::unique_ptr<Command> Attack_command::clone() const {
    return std::make_unique<Attack_command>();
}

std::string Attack_command::get_name() const {
    return "attack";
}

void Switch_mode_command::execute(Game& game) {
    game.handle_switch_mode();
}

std::unique_ptr<Command> Switch_mode_command::clone() const {
    return std::make_unique<Switch_mode_command>();
}

std::string Switch_mode_command::get_name() const {
    return "switch_mode";
}

Cast_spell_command::Cast_spell_command(int index, int x, int y) 
    : spell_index(index), target_x(x), target_y(y) {}

void Cast_spell_command::execute(Game& game) {
    std::cout << "Cast_spell_command: индекс=" << spell_index << ", цель=(" << target_x << "," << target_y << ")\n";
    game.handle_cast_spell(spell_index, target_x, target_y);
}

std::unique_ptr<Command> Cast_spell_command::clone() const {
    return std::make_unique<Cast_spell_command>(spell_index, target_x, target_y);
}

std::string Cast_spell_command::get_name() const {
    return "cast_spell_" + std::to_string(spell_index + 1);
}

Save_command::Save_command(const std::string& file) : filename(file) {}

void Save_command::execute(Game& game) {
    game.handle_save(filename);
}

std::unique_ptr<Command> Save_command::clone() const {
    return std::make_unique<Save_command>(filename);
}

std::string Save_command::get_name() const {
    return "save";
}

Load_command::Load_command(const std::string& file) : filename(file) {}

void Load_command::execute(Game& game) {
    game.handle_load(filename);
}

std::unique_ptr<Command> Load_command::clone() const {
    return std::make_unique<Load_command>(filename);
}

std::string Load_command::get_name() const {
    return "load";
}

void Quit_command::execute(Game& game) {
    game.handle_quit();
}

std::unique_ptr<Command> Quit_command::clone() const {
    return std::make_unique<Quit_command>();
}

std::string Quit_command::get_name() const {
    return "quit";
}

void Heal_command::execute(Game& game) {
    game.handle_heal();
}

std::unique_ptr<Command> Heal_command::clone() const {
    return std::make_unique<Heal_command>();
}

std::string Heal_command::get_name() const {
    return "heal";
}

void Next_level_command::execute(Game& game) {
    game.next_level();
}

std::unique_ptr<Command> Next_level_command::clone() const {
    return std::make_unique<Next_level_command>();
}

std::string Next_level_command::get_name() const {
    return "next_level";
}

void Learn_spell_command::execute(Game& game) {
    game.handle_learn_spell();
}

std::unique_ptr<Command> Learn_spell_command::clone() const {
    return std::make_unique<Learn_spell_command>();
}

std::string Learn_spell_command::get_name() const {
    return "learn_spell";
}