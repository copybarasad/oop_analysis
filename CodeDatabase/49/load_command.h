#ifndef LOAD_COMMAND_H
#define LOAD_COMMAND_H

#include "command_interface.h"

class LoadCommand : public ICommand
{
public:
    LoadCommand() = default;
    CommandType get_type() const override;
};

#endif