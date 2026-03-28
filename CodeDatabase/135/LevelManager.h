#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "LevelConfig.h"

class LevelManager {
private:
	int current_level;

public:
	LevelManager();
	explicit LevelManager(int starting_level);
	
	int getCurrentLevel() const;
	void setLevel(int level);
	void advanceLevel();
	
	LevelConfig getLevelConfig() const;
};

#endif
