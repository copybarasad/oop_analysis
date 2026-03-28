#include "LevelConfig.h"
#include <algorithm>

LevelConfig LevelConfig::Make(int k) {
    LevelConfig c{};
    c.width  = std::min(10 + k, 25);
    c.height = std::min(10 + k, 25);

    c.enemies = std::min(3 + k, 30);
    c.towers  = std::min(1 + k/3, 5);
    c.turrets = std::min(1 + k/2, 6);

    c.enemy_hp     = 6 + 2 * k;
    c.enemy_damage = 1 + (k/2);

    c.turret_hp       = 8 + 3 * k;
    c.turret_damage   = 1 + (k/3);
    c.turret_range    = 2 + (k/3);
    c.turret_cooldown = std::max(2, 5 - 3*k/5);
    c.tower_interval  = std::max(2, 10 - 3*k/5);
    return c;
}
