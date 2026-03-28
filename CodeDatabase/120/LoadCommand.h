#pragma once

#include "ICommand.h"

class LoadCommand : public ICommand {
public:
	bool Execute(GameContext& context) override;
};
