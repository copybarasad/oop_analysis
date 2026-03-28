#include "cast_spell_command.h"

CastSpellCommand::CastSpellCommand(int index, int x, int y)
    : spell_index(index), target_x(x), target_y(y) {}

CommandType CastSpellCommand::get_type() const
{
    return CommandType::CAST_SPELL;
}

int CastSpellCommand::get_spell_index() const
{
    return spell_index;
}

int CastSpellCommand::get_target_x() const
{
    return target_x;
}

int CastSpellCommand::get_target_y() const
{
    return target_y;
}