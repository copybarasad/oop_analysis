#include "pauseMenu.h"

PauseMenu::PauseMenu()
{
	availableOptions.push_back("Save game.");
	availableOptions.push_back("Load game.");
	availableOptions.push_back("Return to game.");
	availableOptions.push_back("Exit to menu.");
}
PauseMenu& PauseMenu::getInstance()
{
	static PauseMenu instance;
    return instance;
}
void PauseMenu::showMenu()
{
	std::cout << "Pause Menu" << std::endl;
	for(size_t i = 0; i < availableOptions.size(); ++i)
	{
		std::cout << i << ": " << availableOptions[i] << std::endl;
	}
}
void PauseMenu::saveGame(Game* game)
{
	SavesMenuController savesMenuController;
	Serializer::getInstance().saveGame(savesMenuController.getSave(), game);
}

int PauseMenu::getOptionsNum()
{
	return availableOptions.size();
}

PauseOptions PauseMenu::convertOption(int option)
{
	return static_cast<PauseOptions>(option);
}