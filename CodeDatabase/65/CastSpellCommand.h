#pragma once

#include "ICommand.h"

class CastSpellCommand : public ICommand {
public:
    void execute(Game& game) override;
};
