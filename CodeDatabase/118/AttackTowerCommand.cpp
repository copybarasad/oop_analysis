#include "Commands.h"

AttackTowerCommand::AttackTowerCommand(int points)
	: pointsForKilling(points) {
}

void AttackTowerCommand::Execute(GameLogic& logic)
{
	try
	{
		auto target = logic.GetTower();
		logic.PlayerAttack(target, pointsForKilling);
	}
	catch (const GameException&)
	{
		throw;
	}
}