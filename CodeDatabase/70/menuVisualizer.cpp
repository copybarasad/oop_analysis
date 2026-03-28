#include "menuVisualizer.h"

MenuVisualizer::MenuVisualizer(){}

MenuVisualizer& MenuVisualizer::getInstance()
{
	static MenuVisualizer instance;
	return instance;
}

void MenuVisualizer::showMenu(IMenu* menu)
{
	system("cls");
	system("cls");
	menu->showMenu();
}