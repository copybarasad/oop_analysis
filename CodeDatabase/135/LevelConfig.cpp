#include "LevelConfig.h"
#include <algorithm>

LevelConfig::LevelConfig(int level) {
	level_number = level;
	
	field_width = std::min(15 + level * 2, 30);
	field_height = std::min(15 + level * 2, 30);
	
	min_enemies = 3 + level;
	max_enemies = 5 + level * 2;
	
	enemy_base_health = 30 + (level - 1) * 10;
	enemy_base_damage = 3 + (level - 1) * 2;
	
	min_barracks = 1;
	max_barracks = 1 + (level / 2);
	
	barracks_health = 50 + (level - 1) * 20;
	barracks_spawn_cooldown = std::max(3, 5 - level / 3);
	
	score_threshold = 10;
}
