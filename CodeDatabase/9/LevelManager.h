#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "Config.h"
#include "SettingsMenu.h"

class LevelManager {
public:
    LevelManager(int totalLevels, int baseBoardWidth, int baseBoardHeight, Difficulty difficulty);

    void Reset();
    bool GoToNextLevel();

    int CurrentLevel() const;
    int TotalLevels() const;

    int BoardWidth() const;
    int BoardHeight() const;

    int EnemyHp() const;
    int EnemyDamage() const;
    int TowerHp() const;
    int TowerDamage() const;

    int RequiredScore() const;

    void SetDifficulty(Difficulty diff);

    void SetCurrentLevel(int level);
    void SetTotalLevels(int total);

private:
    int total_levels_;
    int current_level_;

    int base_board_width_;
    int base_board_height_;
    Difficulty difficulty_;

    int ApplyDifficultyEnemyHpBase() const;
    int ApplyDifficultyEnemyDamageBase() const;
    int ApplyDifficultyTowerHpBase() const;
    int ApplyDifficultyTowerDamageBase() const;
};

#endif