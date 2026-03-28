#include "Player.h"
#include "GameArea.h"
#include "SpellCard.h"


bool Player::attack(Coords enemy_coords, GameArea& gamearea)
{ 
	bool win = gamearea.apply_damage(enemy_coords, damage_calculation(), coords, true);
	if (win) {
		hand.plus_kill();
		if (experience > experience_for_new_level) {
			up_level();
		}
	}
	return win;
}

void Player::up_level()
{
	double rest_exp = experience - experience_for_new_level;
	level++;
	experience = rest_exp;
	health += (health / 100) * 5;
	damage += (damage / 100) * 5;
}

void Player::upgrade_max_health(int plus_hp)
{
	max_health += plus_hp;
	health = max_health;
}

void Player::up_exp(double input_exp)
{
	experience += input_exp;
}

double Player::get_exp()
{
	return experience;
}

void Player::change_range()
{
	if (melee) {
		melee = false;
		damage -= (damage / 100) * 20;
		return;
	}
	melee = true;
	damage += (damage / 100) * 20;
}

void Player::use_spell(Coords caster, GameArea& gamearea) {
	auto spells = hand.get_spells();
	std::cout << "Your spells: " << hand.print() << "\n";
	std::cout << "Upgrades available: " << hand.get_upgrades() << "\n";
	std::cout << "Choose spell (0-" << (spells.size() - 1) << "): ";

	int spell_index = 0;
	std::cin >> spell_index;

	if (spell_index < 0 || spell_index >= spells.size()) return;

	SpellContext context(
		caster,
		{ "Enemy", "EnemyBuilding", "Tower" },
		&hand
	);

	SpellCard* spell = spells[spell_index];
	bool killed = spell->apply(context, gamearea);

	if (killed) {
		hand.plus_kill();
	}
}


int Player::get_count_kill()
{
	return hand.get_count_kill();
}

void Player::clear_half_hand() 
{
	hand.clear_half_hand();
}