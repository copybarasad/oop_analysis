#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "game_command.h"

class Game;

// Шаблонный класс управления игрой.
template <typename CommandReader>
class GameController {
public:
	explicit GameController(CommandReader reader = CommandReader{})
		: reader(std::move(reader)) {}

	// Получает следующую команду от CommandReader
	GameCommand nextCommand() {
		return reader.readCommand();
	}

	// Применяет команду к игре, вызывая соответствующий метод у объекта Game
	void applyCommand(Game& game, const GameCommand& command) {
		game.processCommand(command);
	}

private:
	CommandReader reader;  // Объект класса считывания ввода, созданный из параметра шаблона
};

#endif

