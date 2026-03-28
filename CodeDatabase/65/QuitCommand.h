#pragma once

#include "ICommand.h"

class QuitCommand : public ICommand {
public:
    void execute(Game& game) override;
};
