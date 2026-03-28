#include "h/Game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

int main()
{
	// Initialize random seed for enemy AI and generation
	srand(static_cast<unsigned int>(time(nullptr)));
	
	// Create game with 15x15 field, player with 100 HP and 20 damage
	Game game(15, 15, 100, 20);
	
	// Generate random enemies based on field size
	game.generateRandomEnemies();
	
	// Start main menu loop
	while (true) {
		try {
			game.runMainMenu();
			break; // Exit if user chooses to quit
		} catch (const std::exception& e) {
			std::cout << "Unexpected error: " << e.what() << std::endl;
			std::cout << "Press Enter to continue...";
			std::cin.get();
		}
	}
	
	return 0;
}
