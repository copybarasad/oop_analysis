#pragma once

#include "ICommand.h"

class CancelCommand : public ICommand {
public:
    void execute(Game& game) override;
};
