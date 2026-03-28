#pragma once
#include "Command.h"

class PauseCommand : public Command {
public:
    void execute() override;
    std::string getName() const override;
};