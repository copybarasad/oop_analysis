#pragma once
#include "Commands/Command.h"


class Game;

class ChangeSpeed : public Command
{
	Game* game;
public:
	ChangeSpeed(Game* game);
	void execute() override;
};