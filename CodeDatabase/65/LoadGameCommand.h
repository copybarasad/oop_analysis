#pragma once

#include "ICommand.h"

class LoadGameCommand : public ICommand {
public:
    void execute(Game& game) override;
};
