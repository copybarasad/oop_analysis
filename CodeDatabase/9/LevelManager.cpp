#include "LevelManager.h"

LevelManager::LevelManager(int totalLevels, int baseBoardWidth, int baseBoardHeight, Difficulty difficulty){
    total_levels_ = totalLevels;
    current_level_ = 1;
    base_board_width_ = baseBoardWidth;
    base_board_height_ = baseBoardHeight;
    difficulty_ = difficulty;
}

void LevelManager::Reset() {
    current_level_ = 1;
}

bool LevelManager::GoToNextLevel() {
    if (current_level_ >= total_levels_) {
        return false;
    }
    ++current_level_;
    return true;
}

int LevelManager::CurrentLevel() const {
    return current_level_;
}

int LevelManager::TotalLevels() const {
    return total_levels_;
}

int LevelManager::BoardWidth() const {
    int delta = current_level_ - 1;
    int w = base_board_width_ + delta;
    if (w > 25) w = 24;
    return w;
}

int LevelManager::BoardHeight() const {
    int delta = current_level_ - 1;
    int h = base_board_height_ + delta;
    if (h > 25) h = 24;
    return h;
}

void LevelManager::SetDifficulty(Difficulty diff) {
    difficulty_ = diff;
}


int LevelManager::ApplyDifficultyEnemyHpBase() const {
    switch (difficulty_) {
    case Difficulty::Easy: return ENEMY_HP * 3 / 4;
    case Difficulty::Normal: return ENEMY_HP;
    case Difficulty::Hard: return ENEMY_HP * 3 / 2;
    }
    return ENEMY_HP;
}

int LevelManager::ApplyDifficultyEnemyDamageBase() const {
    switch (difficulty_) {
    case Difficulty::Easy: return ENEMY_DAMAGE * 3 / 4;
    case Difficulty::Normal: return ENEMY_DAMAGE;
    case Difficulty::Hard: return ENEMY_DAMAGE * 3 / 2;
    }
    return ENEMY_DAMAGE;
}

int LevelManager::ApplyDifficultyTowerHpBase() const {
    switch (difficulty_) {
    case Difficulty::Easy: return TOWER_HP * 3 / 4;
    case Difficulty::Normal: return TOWER_HP;
    case Difficulty::Hard: return TOWER_HP * 3 / 2;
    }
    return TOWER_HP;
}

int LevelManager::ApplyDifficultyTowerDamageBase() const {
    switch (difficulty_) {
    case Difficulty::Easy: return TOWER_DAMAGE * 3 / 4;
    case Difficulty::Normal: return TOWER_DAMAGE;
    case Difficulty::Hard: return TOWER_DAMAGE * 3 / 2;
    }
    return TOWER_DAMAGE;
}

int LevelManager::EnemyHp() const {
    int base = ApplyDifficultyEnemyHpBase();
    int factor = 100 + 20 * (current_level_ - 1);
    return base * factor / 100;
}

int LevelManager::EnemyDamage() const {
    int base = ApplyDifficultyEnemyDamageBase();
    int factor = 100 + 20 * (current_level_ - 1);
    return base * factor / 100;
}

int LevelManager::TowerHp() const {
    int base = ApplyDifficultyTowerHpBase();
    int factor = 100 + 20 * (current_level_ - 1);
    return base * factor / 100;
}

int LevelManager::TowerDamage() const {
    int base = ApplyDifficultyTowerDamageBase();
    int factor = 100 + 20 * (current_level_ - 1);
    return base * factor / 100;
}

int LevelManager::RequiredScore() const {
    return 100 * current_level_;
}


void LevelManager::SetTotalLevels(int total) {
    if (total < 1) {
        total = 1;
    }
    total_levels_ = total;
    if (current_level_ > total_levels_) {
        current_level_ = total_levels_;
    }
}

void LevelManager::SetCurrentLevel(int level) {
    if (level < 1) {
        level = 1;
    }
    if (level > total_levels_) {
        level = total_levels_;
    }
    current_level_ = level;
}