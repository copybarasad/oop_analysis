// EnemyTower.cpp
#include "EnemyTower.h"
#include "Spells/DirDamageSpell.h"



EnemyTower::EnemyTower(int radius, int damage)
	: Entity() {
	can_hit_on_walking_ = false;
	can_move_on_cell_ = false;
	spell_ = std::make_shared<DirDamageSpell>(radius, damage);
}

EnemyTower::EnemyTower() : EnemyTower(2, 5) {};

bool EnemyTower::IsFriendly() const {
	return false;
}

std::shared_ptr<ISpell> EnemyTower::canAttack() {
	if (current_tick_ != 0) {
		current_tick_--;
		return nullptr;
	}
	current_tick_ = base_tick_;
	return spell_;
}

TokenEntity EnemyTower::serialise() const {
	TokenEntity token;
	token.type = "EnemyTower";
	token.hp = hp_;
	token.damage = damage_;
	token.can_move_on_cell = can_move_on_cell_;
	token.can_hit_on_walking = can_hit_on_walking_;
	token.not_move_next_turn = not_move_next_turn_;
	token.current_tick = current_tick_;
	token.base_tick = base_tick_;

	// Сериализуем спелл башни
	if (spell_) {
		token.spell.type = "DirDmg";
		auto dir_spell = std::dynamic_pointer_cast<DirDamageSpell>(spell_);
		if (dir_spell) {
			token.spell.damage = std::to_string(dir_spell->getDamage());
			token.spell.range = std::to_string(dir_spell->getRange());
		}
	}

	return token;
}