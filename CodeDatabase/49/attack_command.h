#ifndef ATTACK_COMMAND_H
#define ATTACK_COMMAND_H

#include "command_interface.h"

class AttackCommand : public ICommand
{
public:
    AttackCommand() = default;
    CommandType get_type() const override;
};

#endif