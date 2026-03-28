#include "ChangeGame.h"
#include "ChangeState.h"
#include "Interfaces/Game.h"


ChangeGame::ChangeGame(Game* game, Change changeType) : game(game)
{
	switch (changeType)
	{
	case ChangeGame::Change::NEXTLEVEL:
		this->changeType = Change::UNKNOWN;
		break;
	default:
		this->changeType = changeType;
		break;
	}

}


ChangeGame::ChangeGame(Game* game, BUFF buff) : game(game), buff(buff), changeType(Change::NEXTLEVEL) {}


void ChangeGame::execute()
{
	if (changeType == Change::CONTINUEGAME)
	{
		if (game->hasActiveLevel())
		{
			game->continueLevel();
			ChangeState change = ChangeState(game, ChangeState::State::PLAY);
			change.execute();
		}
	}
	else if (changeType == Change::EXITGAME)
	{
		game->stop();
	}
	else if (changeType == Change::LOADGAME)
	{
		if (game->loadGame())
		{
			ChangeState change = ChangeState(game, ChangeState::State::PLAY);
			change.execute();
		}
	}
	else if (changeType == Change::SAVEGAME)
	{
		game->saveGame();
	}
	else if (changeType == Change::RESETLEVEL)
	{
		game->resetLevel();
		ChangeState change = ChangeState(game, ChangeState::State::PLAY);
		change.execute();
	}
	else if (changeType == Change::RESTARTLEVEL)
	{
		game->getExecuteUseCase()->restartGame();
		ChangeState change = ChangeState(game, ChangeState::State::PLAY);
		change.execute();
	}
	else if (changeType == Change::NEXTLEVEL)
	{
		switch (buff)
		{
		case ChangeGame::BUFF::HEALTH:
			game->nextLevel(game->getBuffHealth(), 0);
			break;
		case ChangeGame::BUFF::DAMAGE:
			game->nextLevel(0, game->getBuffDamage());
			break;
		default:
			break;
		}
		ChangeState change = ChangeState(game, ChangeState::State::PLAY);
		change.execute();
	}
	else if (changeType == Change::UPDATEANDSPAWNENTITIES)
	{
		game->getExecuteUseCase()->checkSpawnNewEnemy();
		game->getExecuteUseCase()->executeEntity();
		game->getExecuteUseCase()->refresh();
	}
	else if (changeType == Change::UPDATEENTITIES)
	{
		game->getExecuteUseCase()->executeEntity();
		game->getExecuteUseCase()->refresh();
	}
}
