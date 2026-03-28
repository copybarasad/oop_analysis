#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "InputReader.h"
#include "Command.h"
#include <memory>

class Game;

template<typename InputReaderType>
class GameManager {
private:
	Game* game_;  
	InputReaderType inputReader_; 

public:
	GameManager(Game* game, InputReaderType reader) 
		: game_(game), inputReader_(std::move(reader)) {}

	// Обработка одного хода игрока
	void processInput() {
		if (game_ == nullptr) return;
		
		std::unique_ptr<Command> command = inputReader_.readCommand();
		
		if (command && command->isValid()) {
			command->execute(*game_);
		}
	}

	Game* getGame() const { return game_; }
};

#endif 





