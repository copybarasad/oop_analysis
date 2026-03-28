#ifndef MAINMENUCONTROLLER_H
#define MAINMENUCONTROLLER_H 

#include <iostream>
#include "mainMenu.h"
#include "menuController.h"
#include "menuVisualizer.h"
#include "inputGetter.h"

class MainMenuController : public MenuController
{
private:
	MainMenu mainMenu;
public:
	MainMenuController();
	void menuCycle();
};

#endif