#pragma once

enum class GameState {
	Menu,
	Playing,
	LevelComplete,
	GameOver,
	Exiting
};

class GameStateManager {
public:
	GameStateManager();
	
	GameState getState() const noexcept { return state; }
	void setState(GameState newState) { state = newState; }
	
	int getCurrentLevel() const noexcept { return currentLevel; }
	void setCurrentLevel(int level) { currentLevel = level; }
	
	int getKillsRequired() const noexcept { return killsRequired; }
	void setKillsRequired(int kills) { killsRequired = kills; }
	
	int getKillsForReward() const noexcept { return killsForReward; }
	void setKillsForReward(int kills) { killsForReward = kills; }
	void resetKillsForReward() noexcept { killsForReward = 0; }
	
	static constexpr int MAX_LEVEL = 5;
	static constexpr int KILLS_FOR_NEW_SPELL = 2;

private:
	GameState state;
	int currentLevel;
	int killsRequired;
	int killsForReward;
};









