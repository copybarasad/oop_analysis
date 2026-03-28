#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <string>

enum class MenuAction {
	CONTINUE,
	SAVE_GAME,
	LOAD_GAME,
	EXIT_GAME,
	START_NEW_GAME,
	CANCEL
};

class GameMenu {
public:
	GameMenu() = default;
	
	MenuAction showPauseMenu();
	MenuAction showStartMenu();
	MenuAction showGameOverMenu();
	
private:
	void clearInputBuffer();
	char getMenuChoice();
	void displayPauseOptions();
	void displayStartOptions();
	void displayGameOverOptions();
};

#endif
