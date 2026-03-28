#include "OpenHandCommand.h"

bool OpenHandCommand::Execute(GameContext& context) {
	context.world.GetPlayer().PrintHand();
    auto spellIdxOpt = context.input_reader.GetSpellIndex(context.world.GetPlayer().GetHand().GetSpells());
    if (!spellIdxOpt) return false;

    size_t idx = *spellIdxOpt;
    auto& spell = context.world.GetPlayer().GetHand().GetSpells()[idx];
    std::optional<Position> targetPos = std::nullopt;

    if (spell->RequiresTarget()) {
        targetPos = context.input_reader.GetTargetPosition();
        if (!targetPos) return false;
    }

    context.logic.PlayerCastSpell(idx, targetPos);
  return true;
}
