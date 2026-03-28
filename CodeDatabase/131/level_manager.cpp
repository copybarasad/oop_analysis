#include "level_manager.h"
#include <algorithm>

namespace rpg {

LevelManager::LevelManager() : current_level_(1) {}

int LevelManager::GetCurrentLevel() const {
    return current_level_;
}

LevelConfig LevelManager::GetLevelConfig(int level) const {
    LevelConfig config;
    config.level_number = level;
    
    config.field_width = std::min(
        GameField::kDefaultFieldSize + static_cast<size_t>(level - 1) * 2,
        GameField::kMaxFieldSize
    );
    config.field_height = config.field_width;
    
    config.enemy_count = 2 + level;
    config.enemy_health_base = 20 + (level - 1) * 10;
    config.enemy_damage_base = 5 + (level - 1) * 3;
    
    config.building_count = 1 + (level - 1) / 2;
    config.tower_count = (level - 1) / 3;
    
    config.impassable_density = std::min(
        GameField::kDefaultImpassableDensity + (level - 1) * 0.02,
        GameField::kMaxImpassableDensity
    );
    config.slowing_density = std::min(
        GameField::kDefaultSlowingDensity + (level - 1) * 0.01,
        GameField::kMaxSlowingDensity
    );
    
    return config;
}

bool LevelManager::CanAdvanceToNextLevel() const {
    return current_level_ < kMaxLevel;
}

void LevelManager::AdvanceToNextLevel() {
    if (CanAdvanceToNextLevel()) {
        ++current_level_;
    }
}

void LevelManager::SetCurrentLevel(int level) {
    current_level_ = level;
}

int LevelManager::GetRequiredScoreForLevel(int level) const {
    return level * 100;
}

bool LevelManager::HasWon(int player_score) const {
    int required_score = GetRequiredScoreForLevel(current_level_);
    return player_score >= required_score;
}

bool LevelManager::IsGameComplete() const {
    return current_level_ > kMaxLevel;
}

LevelConfig LevelManager::GetCurrentLevelConfig() const {
    return GetLevelConfig(current_level_);
}

}


