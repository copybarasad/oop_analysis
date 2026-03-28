#ifndef OPEN_LOAD_MENU_COMMAND_H
#define OPEN_LOAD_MENU_COMMAND_H

#include "command_interface.h"

class OpenLoadMenuCommand : public ICommand {
public:
    bool execute(GameController& game) override;
    std::string getDescription() const override;
    bool isValid() const override;
    CommandType getCommandType() const override { return CommandType::OpenLoadMenu; }
};

#endif