#pragma once
#include "Commands/Command.h"
#include "DataTypes/GameThemes.h"


class Game;

class ChangeTheme : public Command
{
private:
	Game* game;
	GameTheme theme;
public:
	ChangeTheme(Game* game, GameTheme theme);
	void execute() override;
};


