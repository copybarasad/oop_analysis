#pragma once

#include "ICommand.h"

class SaveGameCommand : public ICommand {
public:
    void execute(Game& game) override;
};
