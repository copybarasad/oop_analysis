#pragma once
#include "LevelConfig.h"
#include <map>


class LevelManager {
private:
    std::map<int, LevelConfig> levels;
    int current_level_id;
    int next_level_id;

public:
    LevelManager();
    void loadAllLevels();
    const LevelConfig& getCurrentLevel();
    LevelConfig& getLevel(int id);
    bool nextLevel();
    int getCurrentId() const;
};