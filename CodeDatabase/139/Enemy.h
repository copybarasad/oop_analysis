#pragma once
#include "Entity.h"
#include "JsonLib/json.hpp"


class Enemy : public Entity
{
private:
	unsigned short disappearTime;
public:
	Enemy(int health = 10, int damage = 20, unsigned int disappearTime = 7, unsigned int speed = 1, std::string name = "Enemy");

	void to_json(nlohmann::json& js) const;

	void from_json(const nlohmann::json& js);

	unsigned short getDissapearTime();

	void reduceDissapearTime();

	~Enemy() override = default;
};