#pragma once
#include "icommand.hpp"

class QuitCommand : public ICommand {
public:
    bool Execute(Game& game) override;
};