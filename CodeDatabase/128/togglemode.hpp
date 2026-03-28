#pragma once
#include "icommand.hpp"

class ToggleModeCommand : public ICommand {
public:
    bool Execute(Game& game) override;
};