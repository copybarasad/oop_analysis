#pragma once
#include "command.h"

class SaveCommand : public Command {
public:
    void Execute(GameSession& game) override;
    std::string GetDescription() const override { return "Сохранить игру"; }
};
