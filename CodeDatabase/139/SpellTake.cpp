#include "SpellTake.h"
#include "Interfaces/Game.h"


SpellTake::SpellTake(size_t index) : index(index) {}


void SpellTake::execute(Game* game)
{
	game->getExecuteUseCase()->takeSpell(index);
}