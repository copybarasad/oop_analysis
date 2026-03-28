#ifndef SWITCH_MODE_COMMAND_H
#define SWITCH_MODE_COMMAND_H

#include "command_interface.h"

class SwitchModeCommand : public ICommand {
public:
    bool execute(GameController& game) override;
    std::string getDescription() const override;
    bool isValid() const override;
    CommandType getCommandType() const override { return CommandType::SwitchMode; }
};

#endif