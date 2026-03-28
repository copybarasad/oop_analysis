#include "CastSpellCommand.h"
#include "../Game.h"

namespace Game {

CastSpellCommand::CastSpellCommand(int spellIndex, Utils::Point target) 
    : spellIndex(spellIndex), target(target) {}

void CastSpellCommand::execute(Game& game) {
    game.playerCastSpell(spellIndex, target);
}

}
