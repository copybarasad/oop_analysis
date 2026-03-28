#ifndef SAVESMENUCONTROLLER_H
#define SAVESMENUCONTROLLER_H 

#include <iostream>
#include <string>
#include "menuVisualizer.h"
#include "inputGetter.h"
#include "savesMenu.h"

class MenuVisualizer;

class SavesMenuController
{
public:
	SavesMenuController();
	std::string getSave();
};

#endif