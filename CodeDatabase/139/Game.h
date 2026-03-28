#pragma once
#include "Infrastructure/GameController.h"
#include "Infrastructure/RenderController.h"
#include "UseCases/ExecuteUseCase.h"
#include "States/GameState.h"
#include "DataTypes/GameThemes.h"
#include <string>


class Game
{
private:
	ExecuteUseCase* executeUseCase = nullptr;
	GameState* currentState = nullptr;

	bool isRunning = false; //game
	bool isActiveLevel = false; //started level

	unsigned int width = 800;
	unsigned int height = 600;
	unsigned int cellSize = 15;
	unsigned int rightExpansion = 800;
	unsigned short gameLevelMax = 7;
	unsigned short gameLevel = 1;

	int desiredWidth = 800;
	int desiredHeight = 600;
	bool windowResizeRequested = false;

	GameTheme requestedTheme = GameTheme::DEFAULT;
	bool themeChangeRequested = false;

	bool gameUpdateRequested = false;

	int newLevelBuffHealth = 25;
	int newLevelBuffDamage = 15;

	std::string keyConfigPath = "GameData/PlayKeySettings.json";
public:
	Game(int width, int height, int cellSize = 20);

	void run();

	bool running() const;
	void stop();

	void runActiveLevel();
	void stopActiveLevel();
	bool hasActiveLevel() const;

	void setState(GameState* newState);

	void saveGame() const;
	bool loadGame();
	void nextLevel(int addHealth = 0, int addDamage = 0);
	void resetLevel();
	void continueLevel();

	Point getSize() const;

	unsigned short getGameLevelMax() const;
	unsigned short getGameLevel() const;

	unsigned int fieldWidth() const;
	unsigned int fieldHeight() const;

	void requestWindowResize(int w, int h);
	bool isResizeRequested() const;
	void clearResizeRequest();

	int getDesiredWidth() const;
	int getDesiredHeight() const;

	const std::string& getKeyConfigPath() const;

	void requestThemeChange(GameTheme theme);
	bool isThemeChangeRequested() const;
	void clearThemeChangeRequest();
	GameTheme getRequestedTheme() const;

	void requestUpdateGame();
	bool isUpdateGameRequested() const;
	void clearUpdateGameRequest();

	int getBuffHealth() const;
	int getBuffDamage() const;

	ExecuteUseCase* getExecuteUseCase();
	GameState* getCurrentState();
	~Game();
};