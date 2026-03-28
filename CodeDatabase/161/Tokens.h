#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

struct TokenSpell {
	std::string type;
	std::string damage;
	std::string range;
	std::string area;
	std::string enhancement;
	std::string count_allies;
};

struct TokenHand {
	int limit;
	int upgrade_coef;
	std::vector<TokenSpell> spells;
};

struct TokenWeapon {
	int base_damage;
	int base_attack_radius;
	std::string attack_mode;
};

struct TokenPlayer {
	int hp;
	int score;
	int max_hp;
	TokenWeapon weapon;
	TokenHand hand;
};

struct TokenEntity {
	std::string type;
	int hp;
	int damage;
	bool can_move_on_cell;
	bool can_hit_on_walking;
	bool not_move_next_turn;

	int steps_to_spawn;

	int current_tick;
	int base_tick;
	TokenSpell spell;
};

struct TokenEvent {
	std::string type;
	int damage;
};

struct TokenCell {
	std::string type;
	TokenEntity entity;
	TokenEvent event;
	bool has_entity;
	bool has_event;
};

struct TokenField {
	int width;
	int height;
	std::vector<std::vector<TokenCell>> cells;
};

struct TokenGameState {
	TokenPlayer player;
	TokenField field;
	int player_x;
	int player_y;
	int level;
};

#endif //TOKEN_H