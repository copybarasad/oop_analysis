#include "mainMenu.h"

MainMenu::MainMenu()
{
	availableOptions.push_back("Start new game.");
	availableOptions.push_back("Load game.");
	availableOptions.push_back("Exit game.");
}

void MainMenu::showMenu()
{
	std::cout << "Main Menu" << std::endl;
	for(size_t i = 0; i < availableOptions.size(); ++i)
	{
		std::cout << i << ": " << availableOptions[i] << std::endl;
	}
}

void MainMenu::newGame()
{
	//create Game object
	//give this object to GameController
	Game* game = new Game('P', 'E');
	GameVisualizer<CLIVisualizer> gameVisualizer;
	game->registerObserver(&gameVisualizer);
	game->gameInit();
	GameController<CLIInputHandler> gameController(game);
	gameController.gameCycle(2);
}

bool MainMenu::loadGame()
{
	//go to savesMenu and get Game object
	SavesMenuController savesMenuController;
	Game* game = Serializer::getInstance().loadGame(savesMenuController.getSave());
	if(!game)
	{
		return false;
	}
	GameVisualizer<CLIVisualizer> gameVisualizer;
	game->registerObserver(&gameVisualizer);
	GameController<CLIInputHandler> gameController(game);
	gameController.gameCycle(2);
	return true;
}
void MainMenu::exitGame()
{
	std::cout << "Exiting from game..." << std::endl;
}

int MainMenu::getOptionsNum()
{
	return availableOptions.size();
}

Options MainMenu::convertOption(int option)
{
	return static_cast<Options>(option);
}