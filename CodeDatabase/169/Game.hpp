#pragma once
#include "Field.hpp"
#include "WorldManager.hpp"
#include "Player.hpp"
#include "SaveManager.hpp"
#include "GameException.hpp"
#include "GameStateManager.hpp"
#include "LevelManager.hpp"
#include "CommandHandler.hpp"
#include "GameMenu.hpp"
#include "GameSerializer.hpp"
#include <string>
#include <memory>


class Game {
public:
	Game();
	~Game() = default;
	
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	
	void run();
	
	void saveGame(const std::string& filename);
	void loadGame(const std::string& filename);

private:
	GameStateManager stateManager;
	
	std::unique_ptr<Field> field;
	std::unique_ptr<WorldManager> world;
	Player player;
	
	void gameMenu();
	void startNewGame();
	void startLevel(int level);
	void gameLoop();
	void processPlayerTurn();
	void processEnemiesTurn();
	void processBuildingsTurn();
	void processTowersTurn();
	
	bool isLevelComplete() const;
	bool isGameWon() const;
	void checkGameState();
	
	void clearScreen() const;
	void printGameState() const;
	void checkKillRewards();
	
	bool saveFileExists(const std::string& filename) const;
	
	void handleGameOver();
	void handleLevelComplete();
};

