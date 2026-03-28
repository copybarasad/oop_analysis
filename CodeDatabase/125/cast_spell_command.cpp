#include "cast_spell_command.h"
#include "game_session.h"

CastSpellCommand::CastSpellCommand(int spellIndex) : spellIndex_(spellIndex) {}

void CastSpellCommand::Execute(GameSession& game) {
    game.ProcessSpellCast(spellIndex_);
}

std::string CastSpellCommand::GetDescription() const {
    return "Заклинание " + std::to_string(spellIndex_ + 1);
}
