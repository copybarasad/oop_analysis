#ifndef CLI_H
#define CLI_H

#include "Game.h"
#include <optional>
#include <string>

// CLI class - handles user input and displays game state
class CLI {
private:
	Game& game;

public:
	explicit CLI(Game& game);

	// Display methods (public for Game to use)
	void displayWelcome() const;
	void displayField() const;
	void displayPlayerStats() const;
	void displayCommands() const;
	void displayGameOver() const;
	void displaySpells() const;
	
	// Input handling methods (public for Game to use)
	std::string getUserInput() const;
	std::optional<Direction> parseCommand(const std::string& input) const;
};

#endif // CLI_H
