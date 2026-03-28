#pragma once

#include "ICommand.h"

class SaveCommand : public ICommand {
public:
	bool Execute(GameContext& context) override;
};
