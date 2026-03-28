#include "EnemyTower.hpp"
#include "GameWorld.hpp"
#include "DirectDamageSpell.hpp"
#include <stdexcept>

EnemyTower::EnemyTower(const Position& position,
                       int attackRadius,
                       int damage,
                       int cooldownPeriod,
                       int health)
	: position_(position),
	  attackRadius_(attackRadius),
	  damage_(damage),
	  cooldownRemaining_(0),
	  cooldownPeriod_(cooldownPeriod),
	  health_(health)
{
	if (attackRadius_ <= 0) {
		throw std::invalid_argument("EnemyTower: non-positive radius");
	}
	if (damage_ < 0) {
		throw std::invalid_argument("EnemyTower: negative damage");
	}
	if (cooldownPeriod_ <= 0) {
		throw std::invalid_argument("EnemyTower: non-positive cooldown");
	}
	if (health_ <= 0) {
		throw std::invalid_argument("EnemyTower: non-positive health");
	}
}

EnemyTower::~EnemyTower() {}

const Position& EnemyTower::position() const noexcept {
	return position_;
}

bool EnemyTower::isAlive() const noexcept {
	return health_ > 0;
}

void EnemyTower::takeDamage(int damage) {
	if (damage < 0) {
		throw std::invalid_argument("EnemyTower::takeDamage: negative damage");
	}
	if (!isAlive()) {
		return;
	}
	if (damage >= health_) {
		health_ = 0;
	} else {
		health_ -= damage;
	}
}

void EnemyTower::tickAndAttack(GameWorld& world) {
	if (!isAlive()) {
		return;
	}
	if (cooldownRemaining_ > 0) {
		cooldownRemaining_ -= 1;
		return;
	}

	if (world.isPlayerInRange(position_, attackRadius_)) {
		auto playerPosOpt = world.playerPosition();
		if (playerPosOpt.has_value()) {
			DirectDamageSpell towerSpell(damage_, attackRadius_, /*useStacks*/false, position_);
			SpellBase::SpellContext ctx{
				world,
				world.playerEntity(),
				*playerPosOpt,
				true
			};
			if (towerSpell.cast(ctx)) {
				cooldownRemaining_ = cooldownPeriod_;
			}
		}
	}
}

int EnemyTower::attackRadius() const noexcept {
	return attackRadius_;
}

int EnemyTower::damage() const noexcept {
	return damage_;
}

int EnemyTower::cooldownPeriod() const noexcept {
	return cooldownPeriod_;
}

int EnemyTower::cooldownRemaining() const noexcept {
	return cooldownRemaining_;
}

int EnemyTower::health() const noexcept {
	return health_;
}

void EnemyTower::setCooldownRemaining(int value) noexcept {
	if (value < 0) value = 0;
	cooldownRemaining_ = value;
}

void EnemyTower::setHealth(int value) {
	if (value < 0) {
		throw std::invalid_argument("EnemyTower::setHealth: value < 0");
	}
	health_ = value;
}
