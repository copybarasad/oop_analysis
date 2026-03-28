#pragma once

#include "ICommand.h"

class OpenHandCommand : public ICommand {
public:
	bool Execute(GameContext& context) override;
};
