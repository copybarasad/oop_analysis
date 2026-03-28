#include "cast_spell_command.h"
#include <sstream>

CastSpellCommand::CastSpellCommand(int index, const Position& pos)
	: spellIndex(index), target(pos) {}

bool CastSpellCommand::execute(GameController& game) {
	game.castSpell(spellIndex, target);
	return true;
}

std::string CastSpellCommand::getDescription() const {
	std::ostringstream oss;
	oss << "Cast spell " << spellIndex << " at (" << target.getX() << ", " << target.getY() << ")";
	return oss.str();
}

bool CastSpellCommand::isValid() const {
	return spellIndex >= 0;
}





