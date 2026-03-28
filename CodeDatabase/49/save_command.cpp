#include "save_command.h"

CommandType SaveCommand::get_type() const
{
    return CommandType::SAVE;
}