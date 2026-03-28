#include "Commands.h"

AttackSpawnerCommand::AttackSpawnerCommand(int points)
	: pointsForKilling(points) {}

void AttackSpawnerCommand::Execute(GameLogic& logic)
{
	try
	{
		auto target = logic.GetEnemySpawner();
		logic.PlayerAttack(target, pointsForKilling);
	}
	catch (const GameException&)
	{
		throw;
	}
}