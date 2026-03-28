#pragma once
#include "Commands/Command.h"


class Game;

class ChangeGame : public Command
{
public:
	enum class Change 
	{
		CONTINUEGAME,
		EXITGAME,

		LOADGAME,
		SAVEGAME,

		RESETLEVEL,
		RESTARTLEVEL,
		NEXTLEVEL,

		UPDATEENTITIES,
		UPDATEANDSPAWNENTITIES,

		UNKNOWN,

	};

	enum class BUFF
	{
		HEALTH,
		DAMAGE
	};

private:
	Game* game;
	Change changeType;
	BUFF buff = BUFF::HEALTH;
public:
	ChangeGame(Game* game, Change changeType);
	ChangeGame(Game* game, BUFF buff);
	void execute() override;
};