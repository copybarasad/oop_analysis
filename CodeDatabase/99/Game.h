#ifndef GAME_H
#define GAME_H

#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "GameSaveManager.h"
#include <vector>
#include <memory>

// GameState enum - represents the current state of the game
enum class GameState {
	Running,
	PlayerWon,
	PlayerLost
};

// Game class - manages the game loop and coordinates all components
class Game {
private:
	Field field;
	Player player;
	std::vector<std::unique_ptr<Enemy>> enemies;
	GameState state;
	GameSaveManager saveManager;
	int currentLevel;

	Direction getRandomDirection(const std::vector<Direction>& validDirs);
	void resetGame();
	bool askPlayAgain() const;
	void startNextLevel();

public:
	Game(int fieldWidth, int fieldHeight, int playerHP, int playerDamage);

	// Game loop
	void run();
	void runMainMenu();

	// Save/Load operations
	void saveGame(const std::string& saveName) const;
	bool loadGame(const std::string& saveName);
	void showSaveMenu() const;
	bool showLoadMenu();
	void deleteSave(const std::string& saveName) const;

	// State queries
	GameState getState() const;
	const Player& getPlayer() const;
	const Field& getField() const;
	int getCurrentLevel() const;
	const std::vector<std::unique_ptr<Enemy>>& getEnemies() const;

	// Actions
	void handlePlayerAction(Direction dir);
	void handleSpellCast();
	void handleBuySpell();
	void addEnemy(int hp, int damage, const Position& pos);
	void generateRandomEnemies();  // Generates random enemies based on field size
	
	// Game state management (public for CLI access)
	void processEnemyTurns();
	void checkWinCondition();
	void checkLoseCondition();
	void removeDeadEnemies();
	
	// Save/Load state restoration (for GameSaveManager)
	void restoreGameState(GameState newState, int level);
	void restorePlayer(int hp, int damage, int score, const Position& pos);
	void clearEnemies();
	void restoreEnemy(int hp, int damage, const Position& pos);
};

#endif // GAME_H
