#pragma once
#include "player.h"
#include "playing_field.h"
#include "enemy.h"
#include <memory>
#include <vector>
#include "input_handler.h" 
#include "game_renderer.h"
#include "command.h"

class GameSaver;

class Game {
public:
	Game();
	~Game() = default;
	void setupNewGame();
	void loadExistingGame(const std::string& filename);
	bool processPlayerCommand(const Command& command);
	void handleEnemyTurn();

	const PlayingField* getField() const;
	Player* getPlayer() const;
	bool isRunning() const;
	bool areAllEnemiesDefeated() const;
	bool updateGameState();


private:

	friend class GameSaver;

	void startNextLevel();
	bool handleShop();
	void createAndPlaceEnemy(int health, int damage, const std::string& name, int x, int y);

	void saveCurrentGame(const std::string& filename);
	void resetGame();

	std::unique_ptr<PlayingField> field_;
	std::unique_ptr<Player> player_;
	std::vector<std::unique_ptr<Enemy>> enemies_;

	bool is_running_;
	int current_level_;
};