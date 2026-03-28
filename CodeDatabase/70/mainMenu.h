#ifndef MAINMENU_H
#define MAINMENU_H

#include "imenu.h"
#include "../serializer.h"
#include "../game.h"
#include "../gameController.h"
#include "savesMenuController.h"
#include "../CLIInputHandler.h"
#include "../CLIVisualizer.h"
#include "../gameVisualizer.h"
#include <vector>
#include <string>
#include <iostream>

enum class Options 
{
	NEWGAME,
	LOADGAME,
	EXITGAME
};

class MainMenu : public IMenu
{
private:
	std::vector<std::string> availableOptions;
public:
	MainMenu();
	void showMenu();
	void newGame();
	bool loadGame();
	void exitGame();
	int getOptionsNum();
	Options convertOption(int option);


	//void openSettings();
};
#endif