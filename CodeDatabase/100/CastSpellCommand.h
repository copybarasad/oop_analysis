#pragma once
#include "Command.h"

class CastSpellCommand : public Command {
public:
    void execute() override;
    std::string getName() const override;
};