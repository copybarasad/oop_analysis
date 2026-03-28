#ifndef GAME_H
#define GAME_H

#include <memory>
#include <string>
#include "GameField.h"
#include "GamePersistence.h"

class Player;

class Game {
public:
	Game();

	void run();

private:
	struct DirectionOffset {
		int dx;
		int dy;
		bool valid;
	};

	static constexpr int BASE_FIELD_WIDTH = 15;
	static constexpr int BASE_FIELD_HEIGHT = 15;
	static constexpr int FIELD_GROWTH_STEP = 2;
	static constexpr int MAX_SAVE_SLOTS = 5;

	std::unique_ptr<GameField> field;
	GamePersistence persistence;
	int currentLevel;
	bool exitRequested;
	int currentSaveSlot;

	void showWelcomeMessage() const;
	bool handleStartMenu();
	bool startNewGame();
	bool loadFromStorage();
	void mainLoop();
	void renderState() const;
	void showCommands() const;
	bool processCommand();
	bool executePlayerAction(char command, bool& actionPerformed);
	void executeMovement(char command, bool& actionPerformed);
	void executeAttack(bool& actionPerformed);
	void executeSpell(bool& actionPerformed);
	void openSpellShop(bool& actionPerformed);
	void handleSave();
	void handleLoadDuringGame();
	DirectionOffset requestDirection(const std::string& prompt);
	void advanceNonPlayerTurns();
	void handlePlayerDefeat();
	void handleLevelCompletion();
	void populateLevel(GameField& gameField, int level) const;
	void offerLevelUpOptions();
	void upgradeSpell(Player& player);
	int requestMenuChoice(int minValue, int maxValue, const std::string& prompt);
	void resetGameState(int level, const PlayerState* state);
	void displaySaveSlotsStatus() const;
	std::string buildSavePath(int slot) const;
	bool loadFromSlot(int slot);
	int requestSaveSlot(const std::string& prompt, bool allowCancel);
};

#endif

