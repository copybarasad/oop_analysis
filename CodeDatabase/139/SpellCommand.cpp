#include "SpellCommnad.h"
#include "Commands/System/ChangeGame.h"
#include "Interfaces/Game.h"


SpellCommand::SpellCommand(Game* game, SpellType type) : 
	game(game), type(type), action(SpellAction::BUY) {}


SpellCommand::SpellCommand(Game* game, Point destination) :
	game(game), destination(destination), action(SpellAction::USE) {}


SpellCommand::SpellCommand(Game* game, size_t index) :
	game(game), index(index), action(SpellAction::TAKE) {}


void SpellCommand::execute()
{
	if (action == SpellAction::BUY)
	{
		game->getExecuteUseCase()->addSpell(type);
	}
	else if (action == SpellAction::TAKE)
	{
		game->getExecuteUseCase()->takeSpell(index);
	}
	else if (action == SpellAction::USE)
	{
		if (game->getExecuteUseCase()->useSpell(destination))
		{
			ChangeGame update = ChangeGame(game, ChangeGame::Change::UPDATEENTITIES);
			update.execute();
		}
	}
}