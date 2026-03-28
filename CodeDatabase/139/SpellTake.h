#pragma once
#include "Commands/Command.h"


class SpellTake : public Command
{
private:
	size_t index;

public:
	SpellTake(size_t index);
	void execute(Game* game) override;
};