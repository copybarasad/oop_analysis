#include "Commands.h"

MoveCommand::MoveCommand(std::pair<int, int> steps)
	: steps(steps) {}

void MoveCommand::Execute(GameLogic& logic)
{
	try
	{
		logic.PlayerMove(steps);
	}
	catch (const GameException&)
	{
		throw;
	}
}