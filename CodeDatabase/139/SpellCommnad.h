#pragma once
#include "Commands/Command.h"
#include "DataTypes/Point.h"
#include "DataTypes/SpellType.h"


class Game;

class SpellCommand : public Command
{
private:
	enum class SpellAction
	{
		BUY,
		USE,
		TAKE,
	};

	Game* game;

	SpellAction action;
	SpellType type;
	Point destination;
	size_t index;
public:
	SpellCommand(Game* game, SpellType type);
	SpellCommand(Game* game, Point destination);
	SpellCommand(Game* game, size_t index);
	void execute() override;
};