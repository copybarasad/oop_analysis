#pragma once

struct LevelConfig {
    int width;
    int height;
    int enemies;
    int towers;
    int turrets;

    int enemy_hp;
    int enemy_damage;

    int turret_hp;
    int turret_damage;
    int turret_range;
    int turret_cooldown;
    int tower_interval;

    static LevelConfig Make(int level_index);
};
