#include "../h/Game.h"
#include "../h/CLI.h"
#include "../h/GameExceptions.h"

#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

// Game constructor - initializes field, player, and game state
Game::Game(int fieldWidth, int fieldHeight, int playerHP, int playerDamage)
	: field(fieldWidth, fieldHeight), player(playerHP, playerDamage, 5), state(GameState::Running), currentLevel(1) {
	// Place player at center of field
	Position playerPos = {fieldWidth / 2, fieldHeight / 2};
	field.placeUnit(&player, playerPos);
}

// Processes turns for all enemies
void Game::processEnemyTurns() {
	Position playerPos = field.getUnitPosition(&player);
	
	for (auto& enemy : enemies) {
		Position enemyPos = field.getUnitPosition(enemy.get());
		
		// Get valid directions for enemy
		vector<Direction> validDirs = field.getValidDirections(enemyPos);
		
		// Check all adjacent positions for player
		vector<Direction> allDirs = {Direction::Up, Direction::Down, Direction::Left, Direction::Right};
		for (Direction dir : allDirs) {
			Position adjacent = field.getAdjacentPosition(enemyPos, dir);
			if (adjacent == playerPos) {
				// Attack player
				player.takeDamage(enemy->getDamage());
				return;
			}
		}
		
		// If no valid moves, skip turn
		if (validDirs.empty()) {
			continue;
		}
		
		// Move in random valid direction
		Direction chosenDir = getRandomDirection(validDirs);
		field.moveUnit(enemy.get(), chosenDir);
	}
}

// Checks if player has won (all enemies defeated)
void Game::checkWinCondition() {
	if (enemies.empty()) {
		cout << "\n=== Level " << currentLevel << " Complete! ===" << endl;
		cout << "Score: " << player.getScore() << endl;
		
		// Ask if player wants to continue to next level with validation
		while (true) {
			cout << "Continue to next level? (y/n): ";
			string input;
			getline(cin, input);
			
			// Validate input
			if (input.empty()) {
				cout << "Please enter 'y' for yes or 'n' for no." << endl;
				continue;
			}
			
			// Check first character and normalize to uppercase
			char choice = input[0];
			if (choice >= 'a' && choice <= 'z') {
				choice = choice - 'a' + 'A';
			}
			
			if (choice == 'Y') {
				startNextLevel();
				break;
			} else if (choice == 'N') {
				state = GameState::PlayerWon;
				break;
			} else {
				cout << "Invalid input! Please enter 'y' for yes or 'n' for no." << endl;
			}
		}
	}
}

// Checks if player has lost (HP reached zero)
void Game::checkLoseCondition() {
	if (!player.isAlive()) {
		state = GameState::PlayerLost;
	}
}

// Removes all dead enemies from field and enemies vector
void Game::removeDeadEnemies() {
	// Iterate through enemies and remove dead ones
	for (auto it = enemies.begin(); it != enemies.end(); ) {
		Enemy* enemy = it->get();
		
		if (!enemy->isAlive()) {
			// Increase player score
			player.addScore(10);
			
			// Remove enemy from field
			field.removeUnit(enemy);
			
			// Remove from enemies vector
			it = enemies.erase(it);
		} else {
			++it;
		}
	}
}

// Selects random direction from valid options
// Note: This should never be called with empty validDirs due to check in processEnemyTurns
Direction Game::getRandomDirection(const vector<Direction>& validDirs) {
	// Safety check - should never happen due to caller's validation
	if (validDirs.empty()) {
		// Return any direction as fallback (won't be used due to earlier check)
		return Direction::Up;
	}
	
	int index = rand() % validDirs.size();
	return validDirs[index];
}

// Main menu with save/load options
void Game::runMainMenu() {
	while (true) {
		cout << "\n=== Turn-Based Game ===" << endl;
		cout << "1. New Game" << endl;
		cout << "2. Load Game" << endl;
		cout << "3. Exit" << endl;
		cout << "Choose option: ";
		
		string input;
		getline(cin, input);
		
		if (input.empty()) {
			cout << "Please enter a valid choice." << endl;
			continue;
		}
		
		int choice;
		try {
			choice = stoi(input);
		} catch (const invalid_argument&) {
			cout << "Invalid input! Please enter a number between 1 and 3." << endl;
			continue;
		} catch (const out_of_range&) {
			cout << "Number too large! Please enter a number between 1 and 3." << endl;
			continue;
		}
		
		try {
			switch (choice) {
				case 1:
					resetGame();
					run();
					return; // Exit menu after game ends
				case 2:
					if (showLoadMenu()) {
						run();
						return; // Exit menu after game ends
					}
					// If load was cancelled, continue showing menu
					break;
				case 3:
					cout << "Goodbye!" << endl;
					return;
				default:
					cout << "Invalid choice! Please enter a number between 1 and 3." << endl;
					break;
			}
		} catch (const GameException& e) {
			cout << "Error: " << e.what() << endl;
			cout << "Press Enter to continue...";
			cin.get();
		}
	}
}

// Main game loop - orchestrates game flow
void Game::run() {
	CLI cli(*this);
	
	cli.displayWelcome();
	
	while (state == GameState::Running) {
		// Display current state
		cli.displayField();
		cli.displayPlayerStats();
		cli.displaySpells();
		cli.displayCommands();
		
		// Get user input
		string input = cli.getUserInput();
		
		// Handle special commands
		if (!input.empty()) {
			char cmd = input[0];
			if (cmd >= 'a' && cmd <= 'z') {
				cmd = cmd - 'a' + 'A';
			}
			
			if (cmd == 'Q') {
				cout << "Quitting game..." << endl;
				break;
			} else if (cmd == '0') {
				showSaveMenu();
				continue;
			} else if (cmd == 'C') {
				handleSpellCast();
				// Process enemy turns after spell cast
				processEnemyTurns();
				checkWinCondition();
				checkLoseCondition();
				cout << endl;
				continue;
			} else if (cmd == 'B') {
				handleBuySpell();
				cout << endl;
				continue;  // Buying doesn't consume turn
			}
		}
		
		// Parse movement command
		optional<Direction> dir = cli.parseCommand(input);
		
		if (!dir.has_value()) {
			cout << "Invalid command! Try again." << endl;
			continue;
		}
		
		// Process player action
		handlePlayerAction(dir.value());
		
		// Process enemy turns
		processEnemyTurns();
		
		// Check win/loss conditions
		checkWinCondition();
		checkLoseCondition();
		
		cout << endl;
	}
	
	// Display game over message
	if (state != GameState::Running) {
		cli.displayField();
		cli.displayGameOver();
		
		// Ask if player wants to play again
		if (askPlayAgain()) {
			resetGame();
			run(); // Start new game
		}
	}
}

GameState Game::getState() const {
	return state;
}

const Player& Game::getPlayer() const {
	return player;
}

const Field& Game::getField() const {
	return field;
}

int Game::getCurrentLevel() const {
	return currentLevel;
}

const vector<unique_ptr<Enemy>>& Game::getEnemies() const {
	return enemies;
}

// Restore game state from save file
void Game::restoreGameState(GameState newState, int level) {
	state = newState;
	currentLevel = level;
}

// Restore player state from save file
void Game::restorePlayer(int hp, int damage, int score, const Position& pos) {
	// Remove player from current position
	field.removeUnit(&player);
	
	// Create new player with restored stats
	player = Player(hp, damage, 5);
	player.addScore(score);
	
	// Place player at restored position
	field.placeUnit(&player, pos);
}

// Clear all enemies (for loading)
void Game::clearEnemies() {
	for (auto& enemy : enemies) {
		field.removeUnit(enemy.get());
	}
	enemies.clear();
}

// Restore enemy from save file
void Game::restoreEnemy(int hp, int damage, const Position& pos) {
	auto enemy = make_unique<Enemy>(hp, damage);
	if (field.placeUnit(enemy.get(), pos)) {
		enemies.push_back(move(enemy));
	}
}

// Delete save file
void Game::deleteSave(const string& saveName) const {
	try {
		saveManager.deleteSave(saveName);
		cout << "Save '" << saveName << "' deleted successfully!" << endl;
	} catch (const GameException& e) {
		cout << "Failed to delete save: " << e.what() << endl;
	}
}

// Handles player movement or attack action
void Game::handlePlayerAction(Direction dir) {
	Position playerPos = field.getUnitPosition(&player);
	Position targetPos = field.getAdjacentPosition(playerPos, dir);
	
	// Check if target is out of bounds
	if (!field.isValidPosition(targetPos)) {
		return;
	}
	
	// Check if target has an enemy
	Unit* targetUnit = field.getUnitAt(targetPos);
	if (targetUnit != nullptr) {
		// Attack the enemy
		targetUnit->takeDamage(player.getDamage());
		
		// Check if enemy died
		if (!targetUnit->isAlive()) {
			Game::removeDeadEnemies();
		}
	} else {
		// Move player to empty cell
		field.moveUnit(&player, dir);
	}
}

// Handles spell casting
void Game::handleSpellCast() {
	SpellHand* hand = player.getSpellHand();
	
	if (hand->isEmpty()) {
		cout << "You have no spells!" << endl;
		return;
	}
	
	// Display spells
	cout << "\n=== Spells (" << hand->getCurrentSize() << "/" << hand->getMaxSize() << ") ===" << endl;
	
	if (!hand->isEmpty()) {
		for (int i = 0; i < hand->getCurrentSize(); i++) {
			Spell* spell = hand->getSpell(i);
			cout << i + 1 << ". " << spell->getName() 
			     << " (Range: " << spell->getRange() << ")" << endl;
		}
	}
	
	cout << "Select spell (1-" << hand->getCurrentSize() << ") or 0 to cancel: ";
	int choice;
	if (!(cin >> choice)) {
		// Invalid input (not a number)
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "Invalid input! Must be a number." << endl;
		return;
	}
	cin.ignore(10000, '\n');
	
	if (choice == 0) {
		return;
	}
	
	if (choice < 1 || choice > hand->getCurrentSize()) {
		cout << "Invalid spell selection! Must be between 1 and " << hand->getCurrentSize() << endl;
		return;
	}
	
	Spell* spell = hand->getSpell(choice - 1);
	
	// Get field dimensions for validation
	int maxX = field.getWidth() - 1;
	int maxY = field.getHeight() - 1;
	
	// Input and validate X coordinate
	int x;
	cout << "Enter target X coordinate (0-" << maxX << "): ";
	if (!(cin >> x)) {
		// Invalid input (not a number)
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "Invalid input! Must be a number." << endl;
		return;
	}
	
	if (x < 0 || x > maxX) {
		cin.ignore(10000, '\n');
		cout << "Invalid X coordinate! Must be between 0 and " << maxX << endl;
		return;
	}
	
	// Input and validate Y coordinate
	int y;
	cout << "Enter target Y coordinate (0-" << maxY << "): ";
	if (!(cin >> y)) {
		// Invalid input (not a number)
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "Invalid input! Must be a number." << endl;
		return;
	}
	
	if (y < 0 || y > maxY) {
		cin.ignore(10000, '\n');
		cout << "Invalid Y coordinate! Must be between 0 and " << maxY << endl;
		return;
	}
	
	cin.ignore(10000, '\n');
	
	Position playerPos = field.getUnitPosition(&player);
	Position targetPos = {x, y};
	
	if (spell->cast(field, playerPos, targetPos)) {
		cout << "Spell cast successfully!" << endl;
		hand->removeSpell(choice - 1);
		
		// Remove dead enemies and award score
		removeDeadEnemies();
	} else {
		cout << "Spell cast failed! (out of range or invalid target)" << endl;
		return;  // Don't consume turn if spell failed
	}
}

// Handles buying new spell
void Game::handleBuySpell() {
	const int SPELL_COST = 10;
	
	if (!player.canAffordSpell(SPELL_COST)) {
		cout << "Not enough score! Need " << SPELL_COST << " points." << endl;
		return;
	}
	
	if (player.getSpellHand()->isFull()) {
		cout << "Spell hand is full!" << endl;
		return;
	}
	
	// Deduct cost and buy spell
	player.addScore(-SPELL_COST);
	player.buySpell();
	cout << "New spell purchased!" << endl;
}

// Adds an enemy to the game at specified position
void Game::addEnemy(int hp, int damage, const Position& pos) {
	auto enemy = std::make_unique<Enemy>(hp, damage);
	
	// Try to place enemy on field
	if (field.placeUnit(enemy.get(), pos)) {
		// If successful, add to enemies vector
		enemies.push_back(std::move(enemy));
	}
}

// Generates random enemies based on field dimensions
void Game::generateRandomEnemies() {
	int fieldWidth = field.getWidth();
	int fieldHeight = field.getHeight();
	Position playerPos = field.getUnitPosition(&player);
	
	// Generate 5-7 enemies
	int enemyCount = 5+rand() % 3;
	
	for (int i = 0; i < enemyCount; i++) {
		// Random HP: 20 to 40
		int hp = 20 + rand() % 21;
		// Random damage: 3 to 8
		int damage = 3 + rand() % 6;
		
		// Random position (avoid player position and adjacent cells)
		int x, y;
		int attempts = 0;
		do {
			x = rand() % fieldWidth;
			y = rand() % fieldHeight;
			attempts++;
			
			// Avoid infinite loop if field is too crowded
			if (attempts > 100) {
				break;
			}
		} while (
			// Avoid player position
			(x == playerPos.x && y == playerPos.y) ||
			// Avoid cells adjacent to player (2x2 area around player)
			(x >= playerPos.x - 1 && x <= playerPos.x + 1 && 
			 y >= playerPos.y - 1 && y <= playerPos.y + 1) ||
			// Avoid already occupied cells
			field.isOccupied({x, y})
		);
		
		// Only add if we found a valid position
		if (attempts <= 100) {
			addEnemy(hp, damage, {x, y});
		}
	}
}

// Reset game to initial state
void Game::resetGame() {
	// Reset game state
	state = GameState::Running;
	currentLevel = 1;
	
	// Reset player
	player = Player(100, 20, 5);
	
	// Clear enemies
	enemies.clear();
	
	// Reset field and place player
	field = Field(15, 15);
	Position playerPos = {field.getWidth() / 2, field.getHeight() / 2};
	field.placeUnit(&player, playerPos);
	
	// Generate new enemies
	generateRandomEnemies();
}

// Ask if player wants to play again
bool Game::askPlayAgain() const {
	while (true) {
		cout << "\nDo you want to play again? (y/n): ";
		string input;
		getline(cin, input);
		
		// Validate input
		if (input.empty()) {
			cout << "Please enter 'y' for yes or 'n' for no." << endl;
			continue;
		}
		
		// Check first character and normalize to uppercase
		char choice = input[0];
		if (choice >= 'a' && choice <= 'z') {
			choice = choice - 'a' + 'A';
		}
		
		if (choice == 'Y') {
			return true;
		} else if (choice == 'N') {
			return false;
		} else {
			cout << "Invalid input! Please enter 'y' for yes or 'n' for no." << endl;
		}
	}
}

// Save game with given name
void Game::saveGame(const string& saveName) const {
	try {
		saveManager.saveGame(*this, saveName);
		cout << "Game saved successfully as '" << saveName << "'!" << endl;
	} catch (const GameException& e) {
		cout << "Failed to save game: " << e.what() << endl;
	}
}

// Load game with given name
bool Game::loadGame(const string& saveName) {
	try {
		saveManager.loadGame(*this, saveName);
		cout << "Game loaded successfully from '" << saveName << "'!" << endl;
		return true;
	} catch (const GameException& e) {
		cout << "Failed to load game: " << e.what() << endl;
		return false;
	}
}

// Show save menu
void Game::showSaveMenu() const {
	cout << "\n=== Save Game ===" << endl;
	cout << "Current Level: " << currentLevel << endl;
	cout << "Current Score: " << player.getScore() << endl;
	cout << endl;
	
	while (true) {
		cout << "Choose action:" << endl;
		cout << "1. Save game" << endl;
		cout << "2. Cancel" << endl;
		cout << "Enter choice (1-2): ";
		
		string input;
		getline(cin, input);
		
		// Validate input
		if (input.empty()) {
			cout << "Please enter a valid choice (1 or 2)." << endl;
			continue;
		}
		
		// Try to parse as number
		int choice;
		try {
			choice = stoi(input);
		} catch (const invalid_argument&) {
			cout << "Invalid input! Please enter 1 or 2." << endl;
			continue;
		} catch (const out_of_range&) {
			cout << "Number too large! Please enter 1 or 2." << endl;
			continue;
		}
		
		switch (choice) {
			case 1: {
				// Save game - get save name
				while (true) {
					cout << "\nEnter save name (or '-' to cancel): ";
					
					string saveName;
					getline(cin, saveName);
					
					if (saveName == "-") {
						cout << "Save cancelled." << endl;
						return;
					}
					
					if (saveName.empty()) {
						cout << "Save name cannot be empty. Please enter a valid name." << endl;
						continue;
					}
					
					// Basic validation for save name
					bool validName = true;
					for (char c : saveName) {
						if (c == '/' || c == '\\' || c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|') {
							validName = false;
							break;
						}
					}
					
					if (!validName) {
						cout << "Invalid characters in save name. Please avoid: / \\ : * ? \" < > |" << endl;
						continue;
					}
					
					if (saveName.length() > 50) {
						cout << "Save name too long. Maximum 50 characters allowed." << endl;
						continue;
					}
					
					// Check if save already exists
					if (saveManager.saveExists(saveName)) {
						while (true) {
							cout << "Save '" << saveName << "' already exists. Overwrite? (y/n): ";
							string confirm;
							getline(cin, confirm);
							
							if (confirm.empty()) {
								cout << "Please enter 'y' for yes or 'n' for no." << endl;
								continue;
							}
							
							char overwriteChoice = confirm[0];
							if (overwriteChoice >= 'a' && overwriteChoice <= 'z') {
								overwriteChoice = overwriteChoice - 'a' + 'A';
							}
							
							if (overwriteChoice == 'Y') {
								saveGame(saveName);
								return;
							} else if (overwriteChoice == 'N') {
								break; // Go back to save name input
							} else {
								cout << "Please enter 'y' for yes or 'n' for no." << endl;
							}
						}
					} else {
						saveGame(saveName);
						return;
					}
				}
				break;
			}
			
			case 2:
				cout << "Save cancelled." << endl;
				return;
				
			default:
				cout << "Invalid choice! Please enter 1 or 2." << endl;
				continue;
		}
	}
}

// Show load menu
bool Game::showLoadMenu() {
	cout << "\n=== Load Game ===" << endl;
	
	try {
		vector<string> saves = saveManager.listSaves();
		
		if (saves.empty()) {
			cout << "No saved games found." << endl;
			return false;
		}
		
		while (true) {
			cout << "Available saves:" << endl;
			for (size_t i = 0; i < saves.size(); ++i) {
				cout << (i + 1) << ". " << saves[i] << endl;
			}
			cout << "-. Cancel" << endl;
			cout << "Choose save (enter number or '-' to cancel): ";
			
			string input;
			getline(cin, input);
			
			// Handle cancel input
			if (input == "-" || input.empty()) {
				cout << "Load cancelled." << endl;
				return false;
			}
			
			// Try to parse as number
			int choice;
			try {
				choice = stoi(input);
			} catch (const invalid_argument&) {
				cout << "Invalid input! Please enter a number between 1 and " << saves.size() << " or '-' to cancel." << endl;
				continue;
			} catch (const out_of_range&) {
				cout << "Number too large! Please enter a number between 1 and " << saves.size() << "." << endl;
				continue;
			}
			
			if (choice < 1 || choice > static_cast<int>(saves.size())) {
				cout << "Invalid choice! Please enter a number between 1 and " << saves.size() << "." << endl;
				continue;
			}
			
			string selectedSave = saves[choice - 1];
			
			// Show action menu for selected save
			while (true) {
				cout << "\nSelected save: '" << selectedSave << "'" << endl;
				cout << "Choose action:" << endl;
				cout << "1. Load game" << endl;
				cout << "2. Delete save" << endl;
				cout << "3. Back to save list" << endl;
				cout << "Enter choice (1-3): ";
				
				string actionInput;
				getline(cin, actionInput);
				
				if (actionInput.empty()) {
					cout << "Please enter a valid choice (1, 2, or 3)." << endl;
					continue;
				}
				
				int actionChoice;
				try {
					actionChoice = stoi(actionInput);
				} catch (const invalid_argument&) {
					cout << "Invalid input! Please enter 1, 2, or 3." << endl;
					continue;
				} catch (const out_of_range&) {
					cout << "Number too large! Please enter 1, 2, or 3." << endl;
					continue;
				}
				
				switch (actionChoice) {
					case 1:
						// Load game
						return loadGame(selectedSave);
						
					case 2: {
						// Delete save with confirmation
						cout << "\n=== DELETE CONFIRMATION ===" << endl;
						cout << "You are about to delete save: '" << selectedSave << "'" << endl;
						cout << "This action CANNOT be undone!" << endl;
						
						while (true) {
							cout << "Type exactly 'DELETE' to confirm or anything else to cancel: ";
							
							string confirmation;
							getline(cin, confirmation);
							
							if (confirmation == "DELETE") {
								deleteSave(selectedSave);
								cout << "Save deleted successfully!" << endl;
								
								// Update saves list
								saves = saveManager.listSaves();
								if (saves.empty()) {
									cout << "No more saved games found." << endl;
									return false;
								}
								break; // Go back to save selection
							} else {
								cout << "Delete cancelled." << endl;
								break; // Go back to action menu
							}
						}
						break;
					}
					
					case 3:
						// Back to save list
						break;
						
					default:
						cout << "Invalid choice! Please enter 1, 2, or 3." << endl;
						continue;
				}
				
				if (actionChoice == 2 || actionChoice == 3) {
					break; // Exit action menu, go back to save selection
				}
			}
		}
		
	} catch (const GameException& e) {
		cout << "Error listing saves: " << e.what() << endl;
		return false;
	}
}


// Start next level - preserve player stats and score
void Game::startNextLevel() {
	currentLevel++;
	
	// Clear enemies
	enemies.clear();
	
	// Reset field but keep player position
	Position playerPos = field.getUnitPosition(&player);
	field = Field(15, 15);
	field.placeUnit(&player, playerPos);
	
	// Generate more enemies for higher level
	int enemyCount = 3 + currentLevel;
	for (int i = 0; i < enemyCount; i++) {
		// Stronger enemies on higher levels
		int hp = 20 + (currentLevel * 5) + rand() % 21;
		int damage = 3 + currentLevel + rand() % 6;
		
		// Random position (avoid player position and adjacent cells)
		Position playerPosition = field.getUnitPosition(&player);
		int x, y;
		int attempts = 0;
		do {
			x = rand() % field.getWidth();
			y = rand() % field.getHeight();
			attempts++;
			
			if (attempts > 100) break;
		} while (
			(x == playerPosition.x && y == playerPosition.y) ||
			(x >= playerPosition.x - 1 && x <= playerPosition.x + 1 && 
			 y >= playerPosition.y - 1 && y <= playerPosition.y + 1) ||
			field.isOccupied({x, y})
		);
		
		if (attempts <= 100) {
			addEnemy(hp, damage, {x, y});
		}
	}
	
	cout << "\n=== Level " << currentLevel << " Started! ===" << endl;
	cout << "Enemies: " << enemies.size() << " (stronger than before)" << endl;
}