#include "Command.h"
#include "Game.h"
#include "Direction.h"
#include "MoveUpCommand.h"
#include "MoveDownCommand.h"
#include "MoveLeftCommand.h"
#include "MoveRightCommand.h"
#include "SwitchModeCommand.h"
#include "QuitCommand.h"
#include <iostream>

void MoveUpCommand::execute(Game& game) {
    game.movePlayer(Direction::UP);  
}

void MoveDownCommand::execute(Game& game) {
    game.movePlayer(Direction::DOWN);  
}

void MoveLeftCommand::execute(Game& game) {
    game.movePlayer(Direction::LEFT);  
}

void MoveRightCommand::execute(Game& game) {
    game.movePlayer(Direction::RIGHT);  
}


void SwitchModeCommand::execute(Game& game) {
    game.switchPlayerMode();  
}


void QuitCommand::execute(Game& game) {
    game.quitGame();  
}