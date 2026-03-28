#ifndef SWITCH_MODE_COMMAND_H
#define SWITCH_MODE_COMMAND_H

#include "command_interface.h"

class SwitchModeCommand : public ICommand
{
public:
    SwitchModeCommand() = default;
    CommandType get_type() const override;
};

#endif