#ifndef CAST_SPELL_COMMAND_H
#define CAST_SPELL_COMMAND_H

#include "command_interface.h"

class CastSpellCommand : public ICommand
{
private:
    int spell_index;
    int target_x;
    int target_y;

public:
    CastSpellCommand(int index, int x, int y);

    CommandType get_type() const override;
    int get_spell_index() const;
    int get_target_x() const;
    int get_target_y() const;
};

#endif