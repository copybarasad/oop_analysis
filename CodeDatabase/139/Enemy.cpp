#include "Enemy.h"


Enemy::Enemy(int health, int damage, unsigned int disappearTime, unsigned int speed, std::string name)
	: Entity(health, damage, speed, name), disappearTime(disappearTime) { }


void Enemy::to_json(nlohmann::json& js) const
{
    Entity::to_json(js);

	js["disappearTime"] = disappearTime;
}


void Enemy::from_json(const nlohmann::json& js)
{
    Entity::from_json(js);

	if (js.contains("disappearTime"))
		disappearTime = js["disappearTime"];
	else
		throw std::runtime_error(u8"Ошибка считывания disappearTime");
}


unsigned short Enemy::getDissapearTime() 
{
	return disappearTime;
}


void Enemy::reduceDissapearTime()
{
	--disappearTime;
}