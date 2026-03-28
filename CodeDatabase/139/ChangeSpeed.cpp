#include "ChangeSpeed.h"
#include "Commands/System/ChangeGame.h"
#include "Interfaces/Game.h"


ChangeSpeed::ChangeSpeed(Game* game) : game(game) {}


void ChangeSpeed::execute()
{
	game->getExecuteUseCase()->changePlayerFastMode();
	ChangeGame update = ChangeGame(game, ChangeGame::Change::UPDATEENTITIES);
	update.execute();
}