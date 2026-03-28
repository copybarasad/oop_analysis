#ifndef SAVESMENU_H
#define SAVESMENU_H

#include <vector>
#include <string>
#include "../game.h"
#include "imenu.h"

class SavesMenu : public IMenu
{
private:
	std::vector<std::string> saves;
public:
	SavesMenu();
	static SavesMenu& getInstance();
	void showMenu();
	int getSavesNum();
	std::vector<std::string> getSaves();
};
#endif