#pragma once

#include "ICommand.h"

class MoveCommand : public ICommand {
public:
	explicit MoveCommand(int dx, int dy) : dx_(dx), dy_(dy) {}

	bool Execute(GameContext& context) override;

private:
	int dx_;
	int dy_;
};
