#ifndef OOP_CAST_SPELL_CMD_H
#define OOP_CAST_SPELL_CMD_H

#include "base_command.h"

class CastSpellCommand : public Command{
    int spell_index;
public:
    explicit CastSpellCommand(int spell_idx);
    std::string get_name() override;
    int get_spell_index() const;
};

#endif //OOP_CAST_SPELL_CMD_H