#pragma once
#include "icommand.hpp"

class MoveCommand : public ICommand {
public:
    MoveCommand(int dx, int dy);
    bool Execute(Game& game) override;

private:
    int dx_, dy_;
};