#include "EnemyBuilding.h"

EnemyBuilding::EnemyBuilding()
	: Entity(),
	  steps_to_spawn_(static_cast<int>(SpawnInterval::kDefault)) {
	can_hit_on_walking_ = false;
	can_move_on_cell_ = false;
}

EnemyBuilding::EnemyBuilding(int steps_to_spawn)
	: EnemyBuilding() {
	steps_to_spawn_ = steps_to_spawn >= 0 ? steps_to_spawn : static_cast<int>(SpawnInterval::kDefault);
}

bool EnemyBuilding::CanSpawn() {
	if (steps_to_spawn_ == 0) {
		steps_to_spawn_ = static_cast<int>(SpawnInterval::kDefault);
		return true;
	}
	steps_to_spawn_--;
	return false;
}

bool EnemyBuilding::IsFriendly() const {
	return false;
}

TokenEntity EnemyBuilding::serialise() const {
	TokenEntity token;
	token.type = "EnemyBuilding";
	token.hp = hp_;
	token.damage = damage_;
	token.can_move_on_cell = can_move_on_cell_;
	token.can_hit_on_walking = can_hit_on_walking_;
	token.not_move_next_turn = not_move_next_turn_;
	token.steps_to_spawn = steps_to_spawn_; // ВОТ ОНО!
	return token;
}