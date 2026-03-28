#include "movement_commands.h"
#include "game.h"
#include "player_movement_handler.h"

void MoveUpCommand::execute(Game& game) {
    PlayerMovementHandler::moveUp(game);
}

void MoveDownCommand::execute(Game& game) {
    PlayerMovementHandler::moveDown(game);
}

void MoveLeftCommand::execute(Game& game) {
    PlayerMovementHandler::moveLeft(game);
}

void MoveRightCommand::execute(Game& game) {
    PlayerMovementHandler::moveRight(game);
}