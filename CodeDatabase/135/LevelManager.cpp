#include "LevelManager.h"

LevelManager::LevelManager() : current_level(1) {}

LevelManager::LevelManager(int starting_level) : current_level(starting_level) {}

int LevelManager::getCurrentLevel() const {
	return current_level;
}

void LevelManager::setLevel(int level) {
	if (level >= 1) {
		current_level = level;
	}
}

void LevelManager::advanceLevel() {
	current_level++;
}

LevelConfig LevelManager::getLevelConfig() const {
	return LevelConfig(current_level);
}
