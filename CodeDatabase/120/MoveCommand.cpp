#include "MoveCommand.h"

bool MoveCommand::Execute(GameContext& context) {
	context.logic.MovePlayer(dx_, dy_);
	return true;
}
