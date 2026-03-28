#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Weapon.h"
#include <memory>
#include "Hand.h"

#include "../logic/Tokens.h"

class Hand;

class Player : public Entity {
public:
	Player();
	Player(int, int, int);
	void ChangeAttackMod();
	bool IsFriendly() const override;
	int GetAttackRadius() const;
	int GetHealth() const;
	int GetDamage() const;

	std::shared_ptr<Hand> GetHand();

	int GetScore();
	void minusScore(int);
	void addScore(int);
	bool setHp(int);

	TokenEntity serialise() const override;
	TokenPlayer serialis() const;
	static std::shared_ptr<Player> deserialise(const TokenPlayer&);

	void addMaxHp(int);
	int GetMaxHp();

	std::shared_ptr<Weapon> GetWeapon() const { return weapon_; }
	void UpgradeWeapon(std::shared_ptr<Weapon> new_weapon) {
		weapon_ = new_weapon;
	}

private:
	int score_;
	std::shared_ptr<Weapon> weapon_;
	std::shared_ptr<Hand> hand_;
	int max_hp_;
};

#endif // PLAYER_H