#ifndef EXIT_COMMAND_H
#define EXIT_COMMAND_H

#include "command_interface.h"

class ExitCommand : public ICommand
{
public:
    ExitCommand() = default;
    CommandType get_type() const override;
};

#endif