#pragma once
#include "Entity.h"
#include "Tower.h"
#include "PlayerHand.h"
#include "JsonLib/json.hpp"
#include "Interfaces/ISpellContext.h"


class Player : public Entity
{
private:
	int score = 0;
	int steps = 0;
	bool longFight = false;
	unsigned int longFightDamage;
	unsigned short longFightDistance;
	unsigned short boost = 2;
	bool fastMode = false;
	bool slowed = false;
	PlayerHand* hand = nullptr;
public:
	Player(int health = 100, int damage = 10, unsigned int speed = 1, unsigned short longFightDistance = 2, std::string name = "Hero");

	Player(const Player& other);

	void to_json(nlohmann::json& js) const;

	void from_json(const nlohmann::json& js);

	void addScore(const int score = 1);
	
	void setSlowed(const bool value);

	void setLongFight(const bool value);

	void setFastMode(const bool value);

	void setBoost(const unsigned short boost);

	int getScore() const;

	int getSteps() const;

	float getLongFightDamage() const;

	int getLongFightDistance() const;

	unsigned short getBoost() const;

	const PlayerHand* getHand() const;

	PlayerHand* getHand();

	bool isFastMode() const;

	bool isLongFight() const;
	
	bool isSlowed() const;

	void increaseSteps();

	void move(const Point& vector) override;

	using Entity::attack;

	void attack(Tower& building);

	~Player() override;
};