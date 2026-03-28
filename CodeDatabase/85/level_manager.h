#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

class LevelManager {
private:
    int current_level;
    int max_level_width;
    int max_level_height;

public:
    LevelManager();
    
    void next_level();
    void set_level(int level) { current_level = level; }
    int get_current_level() const { return current_level; }
    int calculate_level_width(int base_width) const;
    int calculate_level_height(int base_height) const;
    int calculate_enemy_health(int base_health) const;
    int calculate_enemy_damage(int base_damage) const;
};

#endif