#include "ChangeTheme.h"
#include "Interfaces/Game.h"


ChangeTheme::ChangeTheme(Game* game, GameTheme theme) : game(game), theme(theme) {}


void ChangeTheme::execute()
{
	game->requestThemeChange(theme);
}