#ifndef MENUVISUALIZER_H
#define MENUVISUALIZER_H

#include "imenu.h"
#include <stdlib.h>

class MenuVisualizer
{
public:
	MenuVisualizer();
	static MenuVisualizer& getInstance();
	void showMenu(IMenu* menu);
};

#endif