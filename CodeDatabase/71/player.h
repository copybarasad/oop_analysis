#pragma once
#include "basic_battler.h"
#include <iostream>
#include "player_hand.h"
class PlayingField;

class Player : public BasicBattler{
public:
	Player(int health, int damage, std::string name, int score, int defense, size_t max_hand_size);
	~Player() = default;

	void takeDamage(int damage_dealt) override;
	int getScore() const;
	int getDefense() const;
	void addScore(int points);

	void processKillReward();
	PlayerHand& getHand();
	void showSpells() const;
	SpellCastResult useSpell(size_t index, PlayingField& field);
	Faction getFaction() const override;

	void heal(int amount);

private:
	int score_;
	int defense_;

	PlayerHand hand_;
	int enemies_defeated_counter_;
};