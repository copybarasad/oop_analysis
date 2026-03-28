#ifndef PAUSEMENUCONTROLLER_H
#define PAUSEMENUCONTROLLER_H 

#include <iostream>
#include "pauseMenu.h"
#include "menuVisualizer.h"
#include "inputGetter.h"
#include "../game.h"

class Visualizer;

class PauseMenuController
{
public:
	PauseMenuController(){}	
	template<typename Controller>
	bool menuCycle(Game* game, Controller* gameController)
	{
		MenuVisualizer::getInstance().showMenu(&PauseMenu::getInstance());
		int option;
		while(true){
			std::cout << "Choose option: ";
			InputGetter::getInstance().getNumberInRange(&option, 0, PauseMenu::getInstance().getOptionsNum());
			PauseOptions input = PauseMenu::getInstance().convertOption(option);
			switch(input)
			{
			case PauseOptions::SAVEGAME:
				PauseMenu::getInstance().saveGame(game);
				MenuVisualizer::getInstance().showMenu(&PauseMenu::getInstance());
				break;
			case PauseOptions::LOADGAME:
				{
					if(!PauseMenu::getInstance().loadGame(game, gameController))
					{
						MenuVisualizer::getInstance().showMenu(&PauseMenu::getInstance());
						std::cout << "Can't load game" << std::endl;
					}
					else MenuVisualizer::getInstance().showMenu(&PauseMenu::getInstance());
					break;
				}
			case PauseOptions::RETURNGAME:
				return true;
				break;
			case PauseOptions::EXITGAME:
				return false;
				break;
			}
		}
		return true;
	}
};



#endif