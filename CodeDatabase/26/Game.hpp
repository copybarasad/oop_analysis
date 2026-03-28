#pragma once

#include <memory>
#include <optional>
#include <random>
#include <string>
#include <vector>
#include "ControlScheme.hpp"
#include "Field.hpp"
#include "GameCommand.hpp"
#include "GameExceptions.hpp"
#include "GameEvent.hpp"
#include "GameWorld.hpp"

class EventBus;

class Game {
public:
	Game(EventBus& events, const ControlScheme& controls);

	bool isExitRequested() const noexcept;
	void handleCommand(const GameCommand& command);

private:
	struct LevelConfig {
		int width;
		int height;
		int enemyHp;
		int enemyDamage;
		int enemyCount;
		int nestCount;
		int towerCount;
		std::size_t handCapacity;
	};

	struct PlayerProgress {
		int maxHealth;
		int meleeDamage;
		int rangedDamage;
		int rangedRange;
		int allyHp;
		int allyDamage;
		int allyCount;
	};

	enum class LevelOutcome {
		PlayerQuit,
		PlayerDefeat,
		PlayerVictory
	};

	enum class Phase {
		MainMenu,
		InLevel
	};

	void initLevels();
	void resetProgress();
	void showMainMenu();
	void startNewCampaign();
	bool promptLoadGame(bool startImmediately = true);
	void promptSaveGame();
	void saveToFile(const std::string& path);
	void loadFromFile(const std::string& path);
	void setupLevel(int levelIndex);
	bool handleMenuCommand(GameCommand::Type type);
	bool handleGameplayCommand(const GameCommand& command);
	void applyTimeStep();
	void dropHalfOfHand();
	void applyUpgrade();
	bool handleDefeatMenu();
	void advanceToNextLevel();
	std::string prompt(const std::string& message) const;
	int randomInt(int minInclusive, int maxInclusive);
	void requestRender() const;
	void publishEvent(const std::string& message, GameEvent::Type type = GameEvent::Type::SystemMessage) const;
	void enterMainMenu();
	std::string keyLabel(GameCommand::Type type, char fallback) const;

	std::vector<LevelConfig> levels_;
	PlayerProgress progress_;
	int currentLevelIndex_;
	std::mt19937 rng_;
	std::unique_ptr<Field> field_;
	std::unique_ptr<GameWorld> world_;
	bool campaignActive_;
	bool exitRequested_;
	std::string lastSavePath_;
	std::vector<int> retainedSpellHotkeys_;
	struct PlayerStatsSnapshot {
		int hp;
		int melee;
		int ranged;
	};
	std::vector<std::optional<PlayerStatsSnapshot>> levelPlayerStats_;
	std::vector<GameWorld::LevelPlanEntry> buildLevelPlan() const;
	EventBus& events_;
	Phase phase_;
	const ControlScheme& controls_;
};
