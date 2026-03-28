#include "savesMenu.h"

SavesMenu::SavesMenu()
{
	saves.push_back("saves/save0");
	saves.push_back("saves/save1");
	saves.push_back("saves/save2");
	saves.push_back("saves/save3");
}
SavesMenu& SavesMenu::getInstance()
{
	static SavesMenu instance;
    return instance;
}
void SavesMenu::showMenu()
{
	for(size_t i = 0; i < saves.size(); ++i)
	{
		std::cout << "Save "<< i << "." << std::endl;
	}
}
int SavesMenu::getSavesNum()
{
	return saves.size();
}
std::vector<std::string> SavesMenu::getSaves()
{
	return saves;
}