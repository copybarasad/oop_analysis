#include "cast_spell_cmd.h"

std::string CastSpellCommand::get_name() {
    return "cast_spell_command";
}

CastSpellCommand::CastSpellCommand(int spell_idx) : spell_index(spell_idx) {}


int CastSpellCommand::get_spell_index() const {
    return spell_index;
}

