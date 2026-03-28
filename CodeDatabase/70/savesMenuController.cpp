#include "savesMenuController.h"


SavesMenuController::SavesMenuController(){}
std::string SavesMenuController::getSave()
{
	MenuVisualizer::getInstance().showMenu(&SavesMenu::getInstance());
	int option;
	while(true){
		std::cout << "Choose save: ";
		InputGetter::getInstance().getNumberInRange(&option, 0, SavesMenu::getInstance().getSavesNum());
		return (SavesMenu::getInstance().getSaves())[option];
	}
}