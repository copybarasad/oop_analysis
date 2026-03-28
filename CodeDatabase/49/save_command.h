#ifndef SAVE_COMMAND_H
#define SAVE_COMMAND_H

#include "command_interface.h"

class SaveCommand : public ICommand
{
public:
    SaveCommand() = default;
    CommandType get_type() const override;
};

#endif