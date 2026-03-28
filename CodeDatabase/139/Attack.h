#pragma once
#include "Commands/Command.h"
#include "DataTypes/Point.h"


class Game;

class Attack : public Command
{
public:
	enum class Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};
private:
	Game* game;
	Point direction;
public:
	Attack(Game* game, Direction direction);
	void execute() override;
};