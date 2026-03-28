#ifndef OOP_LEVEL_MANAGER_H_
#define OOP_LEVEL_MANAGER_H_

#include "game.h"

namespace rpg {

struct LevelConfig {
    int level_number;
    size_t field_width;
    size_t field_height;
    int enemy_count;
    int enemy_health_base;
    int enemy_damage_base;
    int building_count;
    int tower_count;
    double impassable_density;
    double slowing_density;
};

class LevelManager final {
public:
    static constexpr int kMaxLevel = 10;
    
    LevelManager();
    
    [[nodiscard]] int GetCurrentLevel() const;
    
    [[nodiscard]] LevelConfig GetLevelConfig(int level) const;
    
    [[nodiscard]] bool CanAdvanceToNextLevel() const;
    
    void AdvanceToNextLevel();

    void SetCurrentLevel(int level);
    
    [[nodiscard]] bool IsGameComplete() const;

    [[nodiscard]] int GetRequiredScoreForLevel(int level) const;

    [[nodiscard]] bool HasWon(int player_score) const;
    
    [[nodiscard]] LevelConfig GetCurrentLevelConfig() const;

private:
    int current_level_;
};

}

#endif


