#include "MenuController.h"
#include "GameMenu.h"
#include "GameRenderer.h"
#include "GameRestartHandler.h"
#include "InputHandler.h"

MenuController::MenuController(GameMenu& menu, GameRenderer& rend, GameRestartHandler& rh, InputHandler& ih)
	: game_menu(menu), renderer(rend), restart_handler(rh), input_handler(ih) {}

void MenuController::setSaveGameCallback(std::function<void()> callback) {
	save_game_callback = callback;
}

void MenuController::setLoadGameCallback(std::function<void()> callback) {
	load_game_callback = callback;
}

bool MenuController::handlePauseMenu() {
	MenuAction action = game_menu.showPauseMenu();
	
	switch (action) {
		case MenuAction::CONTINUE:
			return true;
		case MenuAction::SAVE_GAME:
			if (save_game_callback) {
				save_game_callback();
			}
			return true;
		case MenuAction::LOAD_GAME:
			if (load_game_callback) {
				load_game_callback();
			}
			return true;
		case MenuAction::EXIT_GAME:
			return false;
		default:
			return true;
	}
}

bool MenuController::handleGameOver(unsigned int& game_turn) {
	renderer.showGameOverScreen();
	
	MenuAction action = game_menu.showGameOverMenu();
	if (action == MenuAction::START_NEW_GAME) {
		restart_handler.restartGame();
		game_turn = 0;
		return true;
	} else {
		return false;
	}
}







