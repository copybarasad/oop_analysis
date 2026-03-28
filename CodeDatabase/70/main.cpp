#include "game.h"
#include "Menus/menuVisualizer.h"
#include "Menus/mainMenuController.h"

int main()
{
	auto& f = SpellsFactory::getInstance();
	f.registerSpell<SpellStraightDamage>("SpellStraightDamage");
	f.registerSpell<SpellAreaDamage>("SpellAreaDamage");
	f.registerSpell<SpellTrap>("SpellTrap");

	MainMenuController mainMenuController;
	mainMenuController.menuCycle();
	return 0;
}