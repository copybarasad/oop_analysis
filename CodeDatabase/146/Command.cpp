#include "Command.h"
#include "Game.h"

MoveCommand::MoveCommand(std::string dir) : direction(dir) {}

void MoveCommand::execute(Game& game) {
    game.move_player(direction);
}

void SpellCommand::execute(Game& game) {
    game.cast_spell_mode();
}

void SaveCommand::execute(Game& game) {
    game.save_current_game();
}

void ExitCommand::execute(Game& game) {
    game.stop_game();
}

void EmptyCommand::execute(Game& game) {

}
