#pragma once

#include "ICommand.h"

class QuitCommand : public ICommand {
public:
	bool Execute(GameContext& context) override;
};
