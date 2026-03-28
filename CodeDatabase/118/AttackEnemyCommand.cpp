#include "Commands.h"

AttackEnemyCommand::AttackEnemyCommand(int points, int number)
	: pointsForKilling(points), numberOfCurrentAttackedEnemy(number) {}

void AttackEnemyCommand::Execute(GameLogic& logic)
{
	std::vector<std::pair<int, int>> enemiesPos = logic.GetEnemyPositions();
	std::shared_ptr<Entity> enemy = logic.GetGameField()->GetCellByPos(enemiesPos[numberOfCurrentAttackedEnemy]).GetCellContent();

	try
	{
		logic.PlayerAttack(enemy, pointsForKilling);
	}
	catch (const GameException&)
	{
		throw;
	}
}