#ifndef OPEN_MENU_COMMAND_H
#define OPEN_MENU_COMMAND_H

#include "command_interface.h"

class OpenMenuCommand : public ICommand {
public:
    bool execute(GameController& game) override;
    std::string getDescription() const override;
    bool isValid() const override;
    CommandType getCommandType() const override { return CommandType::OpenInGameMenu; }
};

#endif