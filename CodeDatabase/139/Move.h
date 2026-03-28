#pragma once
#include "Commands/Command.h"
#include "DataTypes/Point.h"


class Game;

class Move : public Command
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
	Move(Game* game, Direction direction);
	void execute() override;
};