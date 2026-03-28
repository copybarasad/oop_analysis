#include "attack_command.h"

CommandType AttackCommand::get_type() const
{
    return CommandType::ATTACK;
}