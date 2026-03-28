#pragma once

#include "ICommand.h"

class ConfirmCommand : public ICommand {
public:
    void execute(Game& game) override;
};
