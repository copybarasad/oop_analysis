#include "ChangeAttackMode.h"
#include "Commands/System/ChangeGame.h"
#include "Interfaces/Game.h"


ChangeAttackMode::ChangeAttackMode(Game* game) : game(game) {}


void ChangeAttackMode::execute()
{
	game->getExecuteUseCase()->changePlayerLongFight();
	ChangeGame update = ChangeGame(game, ChangeGame::Change::UPDATEENTITIES);
	update.execute();
}