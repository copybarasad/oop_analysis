#include "Attack.h"
#include "Commands/System/ChangeGame.h"
#include "Interfaces/Game.h"


Attack::Attack(Game* game, Direction direction) : game(game)
{
	switch (direction)
	{
	case Attack::Direction::UP:
		this->direction = Point(0, -1);
		break;
	case Attack::Direction::DOWN:
		this->direction = Point(0, 1);
		break;
	case Attack::Direction::LEFT:
		this->direction = Point(-1, 0);
		break;
	case Attack::Direction::RIGHT:
		this->direction = Point(1, 0);
		break;
	default:
		this->direction = Point(0, 0);
		break;
	}
}


void Attack::execute()
{
	if (game->getExecuteUseCase()->executePlayerAttack(direction))
	{
		ChangeGame update = ChangeGame(game, ChangeGame::Change::UPDATEANDSPAWNENTITIES);
		update.execute();
		game->getExecuteUseCase()->increasePlayerSteps();
	}
}