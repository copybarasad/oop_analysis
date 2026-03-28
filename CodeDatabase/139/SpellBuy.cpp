#include "SpellBuy.h"
#include "Interfaces/Game.h"


SpellBuy::SpellBuy(SpellType type) : type(type) {}


void SpellBuy::execute(Game* game)
{
	game->getExecuteUseCase()->addSpell(type);
}