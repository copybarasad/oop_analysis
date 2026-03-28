#include "exit_command.h"

CommandType ExitCommand::get_type() const
{
    return CommandType::EXIT;
}