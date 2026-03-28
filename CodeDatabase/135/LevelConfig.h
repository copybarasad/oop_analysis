#ifndef LEVEL_CONFIG_H
#define LEVEL_CONFIG_H

struct LevelConfig {
	int level_number;
	int field_width;
	int field_height;
	int min_enemies;
	int max_enemies;
	int enemy_base_health;
	int enemy_base_damage;
	int min_barracks;
	int max_barracks;
	int barracks_health;
	int barracks_spawn_cooldown;
	int score_threshold;
	
	LevelConfig(int level);
};

#endif
