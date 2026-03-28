#include "Move.h"
#include "Commands/System/ChangeGame.h"
#include "Interfaces/Game.h"


Move::Move(Game* game, Direction direction) : game(game)
{
	switch (direction)
	{
	case Move::Direction::UP: 
		this->direction = Point(0, -1);
		break;
	case Move::Direction::DOWN:
		this->direction = Point(0, 1);
		break;
	case Move::Direction::LEFT:
		this->direction = Point(-1, 0);
		break;
	case Move::Direction::RIGHT:
		this->direction = Point(1, 0);
		break;
	default:
		this->direction = Point(0, 0);
		break;
	}
}


void Move::execute()
{
	if (game->getExecuteUseCase()->executePlayer(direction))
	{
		ChangeGame update = ChangeGame(game, ChangeGame::Change::UPDATEANDSPAWNENTITIES);
		update.execute();
		game->getExecuteUseCase()->increasePlayerSteps();
	}
}