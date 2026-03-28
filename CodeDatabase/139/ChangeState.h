#pragma once
#include "Commands/Command.h"


class Game;

class ChangeState : public Command
{
public:
	enum class State
	{
		MENU,
		PLAY,
		PAUSE,
		LEVELWIN,
		LEVELBETWEEN,
		GAMEWIN,
		GAMEOVER,
	};
private:
	Game* game;
	State state;
public:
	ChangeState(Game* game, State state);
	void execute() override;
};