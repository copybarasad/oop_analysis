#include "load_command.h"

CommandType LoadCommand::get_type() const
{
    return CommandType::LOAD;
}