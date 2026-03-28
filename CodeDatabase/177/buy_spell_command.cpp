#include "buy_spell_command.h"

bool BuySpellCommand::execute(GameController& game) {
	game.buySpell();
	return true;
}

std::string BuySpellCommand::getDescription() const {
	return "Buy spell";
}

bool BuySpellCommand::isValid() const {
	return true;
}