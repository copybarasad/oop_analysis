#include "Commands.h"
#include "Spells.h"

BuySpellCommand::BuySpellCommand(int number)
	: numberOfSpell(number) {}

void BuySpellCommand::Execute(GameLogic& logic)
{
	std::pair<std::shared_ptr<ISpell>, int> spellWithCost = logic.GetTableOfSpells()[numberOfSpell];

	try
	{
		logic.PlayerBuySpell(spellWithCost.first, spellWithCost.second);
	}
	catch (const GameException&)
	{
		throw;
	}
}