#include "../h/CLI.h"
#include <iostream>

using namespace std;

CLI::CLI(Game& game) : game(game) {
}

// Displays the game field
void CLI::displayField() const {
	cout << game.getField().render();
}

// Displays player statistics
void CLI::displayPlayerStats() const {
	const Player& player = game.getPlayer();
	cout << "Level: " << game.getCurrentLevel() << " | ";
	cout << "HP: " << player.getHP() << " | ";
	cout << "Damage: " << player.getDamage() << " | ";
	cout << "Score: " << player.getScore() << endl;
}

// Displays available commands
void CLI::displayCommands() const {
	cout << "Commands: W/A/S/D (move), C (cast spell), B (buy spell), 0 (save), Q (quit)" << endl;
}

// Displays player's spells
void CLI::displaySpells() const {
	const Player& player = game.getPlayer();
	SpellHand* hand = player.getSpellHand();
	
	cout << "\n=== Spells (" << hand->getCurrentSize() << "/" << hand->getMaxSize() << ") ===" << endl;
	
	if (hand->isEmpty()) {
		cout << "No spells available!" << endl;
		return;
	}
	
	for (int i = 0; i < hand->getCurrentSize(); i++) {
		Spell* spell = hand->getSpell(i);
		cout << i + 1 << ". " << spell->getName() 
		     << " (Range: " << spell->getRange() << ")" << endl;
	}
}

// Displays game over message with final score
void CLI::displayGameOver() const {
	const Player& player = game.getPlayer();
	cout << "\n=== GAME OVER ===" << endl;
	
	if (game.getState() == GameState::PlayerWon) {
		cout << "Congratulations! You won, Champ!" << endl;
	} else {
		cout << "You lost and died! lol" << endl;
	}
	
	cout << "Final Score: " << player.getScore() << endl;
}

// Displays welcome message
void CLI::displayWelcome() const {
	cout << "=== Turn-Based Game ===" << endl;
	cout << "Defeat all enemies to win!" << endl;
	cout << "P = Player, E = Enemy, . = Empty" << endl;
	cout << endl;
}

// Gets user input from console
string CLI::getUserInput() const {
	cout << "\nEnter command: ";
	string input;
	getline(cin, input);
	return input;
}

// Parses user command input (case-insensitive)
optional<Direction> CLI::parseCommand(const string& input) const {
	if (input.empty()) {
		return nullopt;
	}
	
	char cmd = input[0];
	// Convert to uppercase for case-insensitive comparison
	if (cmd >= 'a' && cmd <= 'z') {
		cmd = cmd - 'a' + 'A';
	}
	
	switch (cmd) {
		case 'W':
			return Direction::Up;
		case 'S':
			return Direction::Down;
		case 'A':
			return Direction::Left;
		case 'D':
			return Direction::Right;
		case 'Q':
		case 'C':  // Cast spell
		case 'B':  // Buy spell
			return nullopt;  // Special commands
		default:
			return nullopt;  // Invalid command
	}
}
