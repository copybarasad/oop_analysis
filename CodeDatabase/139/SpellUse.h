#pragma once
#include "Commands/Command.h"
#include "DataTypes/Point.h"


class SpellUse : public Command
{
	Point destination;
public:
	SpellUse(Point destination);
	void execute(Game* game) override;
};