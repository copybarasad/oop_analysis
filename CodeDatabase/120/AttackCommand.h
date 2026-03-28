#pragma once

#include "ICommand.h"

class AttackCommand : public ICommand {
public:
	bool Execute(GameContext& context) override;
};
