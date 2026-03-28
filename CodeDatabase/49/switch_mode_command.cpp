#include "switch_mode_command.h"

CommandType SwitchModeCommand::get_type() const
{
    return CommandType::SWITCH_MODE;
}