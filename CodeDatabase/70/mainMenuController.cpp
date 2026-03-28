#include "mainMenuController.h"

MainMenuController::MainMenuController() : mainMenu(MainMenu()){}
void MainMenuController::menuCycle()
{
	MenuVisualizer menuVisualizer;
	menuVisualizer.showMenu(&mainMenu);
	int option;
	while(true){
		std::cout << "Choose option: ";
		InputGetter::getInstance().getNumberInRange(&option, 0, mainMenu.getOptionsNum());
		Options input = mainMenu.convertOption(option);
		switch(input)
		{
		case Options::NEWGAME:
			mainMenu.newGame();
			menuVisualizer.showMenu(&mainMenu);
			break;
		case Options::LOADGAME:
			if(!mainMenu.loadGame()) 
			{
				menuVisualizer.showMenu(&mainMenu);
				menuVisualizer.showMenu(&mainMenu);
				std::cout << "Can't load game!" << std::endl;
			}
			else menuVisualizer.showMenu(&mainMenu);
			break;
		case Options::EXITGAME:
			mainMenu.exitGame();
			return;
			break;
		}
	}
}