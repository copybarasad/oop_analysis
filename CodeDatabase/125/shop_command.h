#pragma once
#include "command.h"

class ShopCommand : public Command {
public:
    void Execute(GameSession& game) override;
    std::string GetDescription() const override { return "Открыть магазин заклинаний"; }
};
