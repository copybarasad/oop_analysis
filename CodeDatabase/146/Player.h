#pragma once
#include "Human.h"
#include "Hand.h"

class Player : public Human {
private:
	double max_health;
	double experience;
	double experience_for_new_level;
	Hand hand;
	int level;
	bool melee = true;
public:
	Player(double h, double d, Coords coords, int hand_size) : Human(h, d, coords), experience(0), experience_for_new_level(50), level(1), hand(hand_size), max_health(h) {}
	bool attack(Coords enemy_coords, GameArea& gamearea);
	void up_level();
	void upgrade_damage(int plus_dmg) { damage += plus_dmg; };
	void upgrade_max_health(int plus_hp);
	void up_exp(double input_exp);
	double get_exp();
	void change_range();
	void use_spell(Coords caster, GameArea& gamearea);
	int get_count_kill();
	void healing() { health = max_health; };
	Hand* get_hand() { return &hand; };
	void add_slot_hand() { hand.add_slot(); };
	void clear_half_hand();
};
