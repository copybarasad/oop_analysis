#include "invalid_command.h"

InvalidCommand::InvalidCommand(const std::string& error_reason)
    : reason(error_reason) {}

CommandType InvalidCommand::get_type() const
{
    return CommandType::INVALID;
}

const std::string& InvalidCommand::get_reason() const
{
    return reason;
}