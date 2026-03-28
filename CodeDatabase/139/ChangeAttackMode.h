#pragma once
#include "Commands/Command.h"


class Game;

class ChangeAttackMode : public Command
{
	Game* game;
public:
	ChangeAttackMode(Game* game);
	void execute() override;
};