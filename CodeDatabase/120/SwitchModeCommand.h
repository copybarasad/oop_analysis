#pragma once

#include "ICommand.h"

class SwitchModeCommand : public ICommand {
public:
	bool Execute(GameContext& context) override;
};
