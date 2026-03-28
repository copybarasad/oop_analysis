#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "imenu.h"
#include "../game.h"
// #include "../gameController.h"
#include "savesMenuController.h"
#include "../serializer.h"
#include <iostream>
#include <vector>

class Visualizer;
template<typename InputHandler>
class GameController;

enum class PauseOptions 
{
	SAVEGAME,
	LOADGAME,
	RETURNGAME,
	EXITGAME
};

class PauseMenu : public IMenu
{
private:
	std::vector<std::string> availableOptions;
public:
	PauseMenu();
	static PauseMenu& getInstance();
	void showMenu();
	void saveGame(Game* game);
	template<typename Controller>
	bool loadGame(Game* game, Controller* gameController);
	int getOptionsNum();
	PauseOptions convertOption(int option);


	//void openSettings();
};

template<typename Controller>
bool PauseMenu::loadGame(Game* game, Controller* gameController)
{
	SavesMenuController savesMenuController;
    std::string saveFile = savesMenuController.getSave();
    Game* loadedGame = Serializer::getInstance().loadGame(saveFile);
    if (!loadedGame) {
        std::cout << "Failed to load game from: " << saveFile << std::endl;
        return false;
    }
    loadedGame->setObservers(game->getObservers());
    delete game;
    game = loadedGame;
    gameController->setGame(game);
    return true;
}

#endif