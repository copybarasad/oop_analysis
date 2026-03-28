#ifndef MENU_CONTROLLER_H
#define MENU_CONTROLLER_H

#include <functional>

class GameMenu;
class GameRenderer;
class GameRestartHandler;
class InputHandler;
enum class MenuAction;

class MenuController {
private:
	GameMenu& game_menu;
	GameRenderer& renderer;
	GameRestartHandler& restart_handler;
	InputHandler& input_handler;
	
	std::function<void()> save_game_callback;
	std::function<void()> load_game_callback;

public:
	MenuController(GameMenu& menu, GameRenderer& rend, GameRestartHandler& rh, InputHandler& ih);
	
	void setSaveGameCallback(std::function<void()> callback);
	void setLoadGameCallback(std::function<void()> callback);
	
	bool handlePauseMenu();
	
	bool handleGameOver(unsigned int& game_turn);
};

#endif







