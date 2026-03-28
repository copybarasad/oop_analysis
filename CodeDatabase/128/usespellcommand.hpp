#pragma once
#include "icommand.hpp"
#include <cstddef>

class UseSpellCommand : public ICommand {
public:
    UseSpellCommand() = default;
    bool Execute(Game& game) override;
};
