#pragma once
#include "GameContext.h"

class ICommand {
public:
	virtual ~ICommand() = default;
	virtual bool Execute(GameContext& context) = 0;
};
