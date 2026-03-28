#include "Player.h"

Player::Player()
	: Entity(),
	  score_(0),
	  max_hp_(100),
	  weapon_(std::make_shared<Weapon>()) {
	hand_ = std::make_shared<Hand>(5);
	hand_->GiveStartingSpellIfNewGame();
}

Player::Player(int hp, int damage, int attack_radius)
	: Entity(),
	  score_(0),
		max_hp_(100),
	  weapon_(std::make_shared<Weapon>(damage, attack_radius))
{
	hand_ = std::make_shared<Hand>(5);
	hand_->GiveStartingSpellIfNewGame();
	hp_ = hp > 0 ? hp : 100;
}

void Player::ChangeAttackMod() {
	weapon_->ChangeAttackMod();
}

bool Player::IsFriendly() const {
	return true;
}

int Player::GetAttackRadius() const {
	return weapon_->GetAttackRadius();
}

int Player::GetHealth() const {
	return hp_;
}

int Player::GetDamage() const {
	return weapon_->GetDamage();
}

int Player::GetScore() {
	return score_;
}
void Player::minusScore(int sum) {
	if (score_ > sum) {
		score_ -= sum;
	}
}
void Player::addScore(int add) {
	if (add > 0)
		score_ += add;
}

std::shared_ptr<Hand> Player::GetHand() {
	return hand_;
}

TokenPlayer Player::serialis() const {
	TokenPlayer token;

	token.hp = hp_;
	token.score = score_;
	token.max_hp = max_hp_;

	// Weapon
	token.weapon.base_damage = weapon_->GetDamage();
	token.weapon.base_attack_radius = weapon_->GetAttackRadius();
	token.weapon.attack_mode = (weapon_->GetAttackMode() == AttackMod::RangeCombat) ? "Range" : "Close";

	// Hand
	token.hand.limit = hand_->capacity();
	token.hand.upgrade_coef = hand_->isUpgradeEnabled();

	// Spells in hand
	for (size_t i = 0; i < hand_->size(); ++i) {
		auto spell = hand_->getSpell(i);
		if (spell) {
			TokenSpell spell_token = spell->serialise();
			token.hand.spells.push_back(spell_token);
		}
	}

	return token;
}


std::shared_ptr<Player> Player::deserialise(const TokenPlayer& token) {
	auto player = std::make_shared<Player>(token.hp, token.weapon.base_damage, token.weapon.base_attack_radius);

	player->addScore(token.score);

	// Восстанавливаем режим атаки
	if (token.weapon.attack_mode == "Close") {
		player->ChangeAttackMod();
	}

	if (token.max_hp > 100) {
		player->max_hp_ = token.max_hp;
	}

	auto hand = player->GetHand();

	hand->clear();

	for (const auto& spell_token : token.hand.spells) {
		std::shared_ptr<ISpell> spell;

		if (spell_token.type == "DirDmg") {
			int damage = std::stoi(spell_token.damage);
			int range = std::stoi(spell_token.range);
			spell = std::make_shared<DirDamageSpell>(range, damage);
		} else if (spell_token.type == "AreaDmg") {
			int damage = std::stoi(spell_token.damage);
			int area = std::stoi(spell_token.area);
			spell = std::make_shared<AreaDmgSpell>(area, damage);
		} else if (spell_token.type == "Trap") {
			int damage = std::stoi(spell_token.damage);
			spell = std::make_shared<TrapSpell>(damage);
		} else if (spell_token.type == "Enhancement") {
			int enhancement = std::stoi(spell_token.enhancement);
			spell = std::make_shared<EnhacementSpell>(enhancement);
		} else if (spell_token.type == "Summon") {
			int count = std::stoi(spell_token.count_allies);
			spell = std::make_shared<SummSpell>(count);
		}

		if (spell) {
			hand->addSpell(spell);
		}
	}

	return player;
}

bool Player::setHp(int hp) {
	if (hp <= 0)
		return false;

	hp_ = std::min(hp, max_hp_);
	return true;
}

TokenEntity Player::serialise() const {
	return TokenEntity();
}

void Player::addMaxHp(int value) {
	max_hp_ += value;
}

int Player::GetMaxHp() {
	return max_hp_;
}