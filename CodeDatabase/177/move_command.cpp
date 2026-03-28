#include "move_command.h"
#include <sstream>

MoveCommand::MoveCommand(Direction dir) : direction(dir) {}

bool MoveCommand::execute(GameController& game) {
	game.movePlayer(direction);
	return true;
}

std::string MoveCommand::getDescription() const {
	std::string dirStr;
	switch (direction) {
		case Direction::UP: dirStr = "UP"; break;
		case Direction::DOWN: dirStr = "DOWN"; break;
		case Direction::LEFT: dirStr = "LEFT"; break;
		case Direction::RIGHT: dirStr = "RIGHT"; break;
	}
	return "Move " + dirStr;
}

bool MoveCommand::isValid() const {
	return true;
}





