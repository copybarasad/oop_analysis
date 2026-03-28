#pragma once

#include "ICommand.h"

class OpenShopCommand : public ICommand {
public:
	bool Execute(GameContext& context) override;
};
