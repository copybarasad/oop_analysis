#include "Player.h"
#include <iostream>


Player::Player(int health, int damage, unsigned int speed, unsigned short longFightDistance, std::string name)
	: Entity(health, damage, speed, name), longFightDamage(damage * 2), longFightDistance(longFightDistance)
{
	hand = new PlayerHand(pos);
}


Player::Player(const Player& other)
	: Entity(other.health, other.damage, other.speed, other.name), score(other.score),
	steps(other.steps), longFight(other.longFight), longFightDamage(other.longFightDamage),
	longFightDistance(other.longFightDistance), fastMode(other.fastMode), slowed(other.slowed), boost(other.boost)
{
	this->setPosition(other.getPosition());

	hand = new PlayerHand(*other.hand);
}


void Player::to_json(nlohmann::json& js) const
{
	Entity::to_json(js);

	js["score"] = score;
	js["steps"] = steps;
	js["longFight"] = longFight;
	js["longFightDamage"] = longFightDamage;
	js["longFightDistance"] = longFightDistance;
	js["fastMode"] = fastMode;
	js["slowed"] = slowed;
	js["boost"] = boost;

	if (hand) {
		nlohmann::json jHand;
		hand->to_json(jHand);
		js["hand"] = jHand;
	}
}


void Player::from_json(const nlohmann::json& js)
{
	Entity::from_json(js);

	if (js.contains("score"))
		score = js["score"];
	else
		throw std::runtime_error(u8"Отсутствует поле score");

	if (js.contains("steps"))
		steps = js["steps"];
	else
		throw std::runtime_error(u8"Отсутствует поле steps");

	if (js.contains("longFight"))
		longFight = js["longFight"];
	else
		throw std::runtime_error(u8"Отсутствует поле longFight");

	if (js.contains("longFightDamage"))
		longFightDamage = js["longFightDamage"];
	else
		throw std::runtime_error(u8"Отсутствует поле longFightDamage");

	if (js.contains("longFightDistance"))
		longFightDistance = js["longFightDistance"];
	else
		throw std::runtime_error(u8"Отсутствует поле longFightDistance");

	if (js.contains("fastMode"))
		fastMode = js["fastMode"];
	else
		throw std::runtime_error(u8"Отсутствует поле fastMode");

	if (js.contains("slowed"))
		slowed = js["slowed"];
	else
		throw std::runtime_error(u8"Отсутствует поле slowed");

	if (js.contains("boost"))
		boost = js["boost"];
	else
		throw std::runtime_error(u8"Отсутствует поле boost");

	if (hand) {
		delete hand;
		hand = nullptr;
	}

	hand = new PlayerHand({ pos.X(), pos.Y() });
	if (js.contains("hand")) {
		try {
			hand->from_json(js.at("hand"));
		}
		catch (...) {
			throw std::runtime_error(u8"Не удалось считать hand");
		}
	}
	else
		throw std::runtime_error(u8"Отсутствует поле hand");
}


void Player::addScore(const int score)
{
	this->score += score;
}


void Player::setSlowed(const bool value)
{
	slowed = value;
}


void Player::setLongFight(const bool value)
{
	longFight = value;
}


void Player::setFastMode(const bool value)
{
	fastMode = value;
}


void Player::setBoost(const unsigned short boost)
{
	this->boost = boost;
}


int Player::getScore() const
{
	return score;
}


int Player::getSteps() const
{
	return steps;
}


float Player::getLongFightDamage() const
{
	return longFightDamage;
}


int Player::getLongFightDistance() const
{
	return longFightDistance;
}


unsigned short Player::getBoost() const
{
	return boost;
}


const PlayerHand* Player::getHand() const
{
	return hand;
}


PlayerHand* Player::getHand()
{
	return hand;
}


bool Player::isFastMode() const
{
	return fastMode;
}


bool Player::isLongFight() const
{
	return longFight;
}


bool Player::isSlowed() const
{
	return slowed;
}


void Player::increaseSteps()
{
	++steps;
}


void Player::move(const Point& vector)
{	
	pos += vector * (fastMode ? speed *  boost: speed);
	hand->setPosition(pos);
}


void Player::attack(Tower& building) 
{ 
	building.takeDamage(damage);
}


Player::~Player()
{
	if (hand != nullptr)
	{
		delete hand;
		hand = nullptr;
	}
}