#ifndef TURN_RESULTS_H
#define TURN_RESULTS_H

#include <string>
#include <vector>

enum class TurnAction {
	NONE,
	MOVE,
	ATTACK,
	WEAPON_CHANGE,
	SPELL_CAST,
	SHOP_VISIT,
	SLOWED
};

struct AttackResult {
	bool success;
	int target_x;
	int target_y;
	int damage;
	int target_hp_before;
	int target_hp_after;
	int target_max_hp;
	bool target_killed;
	bool is_melee;
	std::string target_type_name;
	int score_gained;
	int resource_points_gained;
	int total_kills;
};

struct MoveResult {
	bool success;
	int old_x;
	int old_y;
	int new_x;
	int new_y;
	std::string direction;
	bool stepped_on_slow_cell;
	bool trap_triggered;
	std::string error_message;
};

struct SpellCastResult {
	bool success;
	bool cancelled;
	std::string error_message;
};

struct WeaponChangeResult {
	bool is_melee;
	int damage;
};

struct PlayerTurnResult {
	TurnAction action;
	MoveResult move;
	AttackResult attack;
	SpellCastResult spell;
	WeaponChangeResult weapon;
	bool player_slowed;
	int player_x;
	int player_y;
};

struct EnemyAction {
	int enemy_x;
	int enemy_y;
	int new_x;
	int new_y;
	bool moved;
	bool attacked;
	int damage_dealt;
	int player_hp_before;
	int player_hp_after;
	int player_max_hp;
};

struct EnemyTurnResult {
	std::vector<EnemyAction> actions;
};

struct AllyAction {
	int ally_x;
	int ally_y;
	int new_x;
	int new_y;
	bool moved;
	bool attacked;
	int target_x;
	int target_y;
	int damage_dealt;
	bool enemy_killed;
};

struct AllyTurnResult {
	std::vector<AllyAction> actions;
};

struct TowerAction {
	int tower_x;
	int tower_y;
	int damage_dealt;
	int player_hp_before;
	int player_hp_after;
	int player_max_hp;
};

struct TowerTurnResult {
	std::vector<TowerAction> actions;
};

struct SpawnEvent {
	int barracks_x;
	int barracks_y;
	int spawned_x;
	int spawned_y;
	bool success;
	std::string message;
};

struct UpdateResult {
	std::vector<SpawnEvent> spawns;
	int entities_removed;
};

#endif
