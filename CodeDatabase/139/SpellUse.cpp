#include "SpellUse.h"
#include "Commands/System/UpdateEntities.h"
#include "Interfaces/Game.h"


SpellUse::SpellUse(Point destination) : destination(destination) {}


void SpellUse::execute(Game* game)
{
	if (game->getExecuteUseCase()->useSpell(destination))
	{
		UpdateEntities update = UpdateEntities();
		update.execute(game);
	}
}