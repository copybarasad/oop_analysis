#pragma once
#include "Commands/Command.h"
#include "DataTypes/SpellType.h"


class SpellBuy : public Command
{
	SpellType type;
public:
	SpellBuy(SpellType type);
	void execute(Game* game) override;
};