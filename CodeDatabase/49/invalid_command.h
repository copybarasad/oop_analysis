#ifndef INVALID_COMMAND_H
#define INVALID_COMMAND_H

#include "command_interface.h"
#include <string>

class InvalidCommand : public ICommand
{
private:
    std::string reason;

public:
    InvalidCommand(const std::string& error_reason = "Unknown error");
    CommandType get_type() const override;
    const std::string& get_reason() const;
};

#endif