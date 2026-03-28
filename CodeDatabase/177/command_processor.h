#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "command_interface.h"
#include "game_controller.h"
#include <memory>
#include <string>

// Прослойка между командами и классом игры
class CommandProcessor {
private:
	GameController* game;
	
public:
	explicit CommandProcessor(GameController* gameController);
	
	// Обработать команду
	bool processCommand(CommandPtr command);
	
	// Выполнить команду
	bool executeCommand(CommandPtr command);
	
	void setGame(GameController* gameController) { game = gameController; }
	GameController* getGame() const { return game; }
};

#endif