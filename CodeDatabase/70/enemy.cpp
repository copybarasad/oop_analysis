#include "enemy.h"

Enemy::Enemy() : Entity(100, 10), damage(10){}
Enemy::Enemy(int health, int damage, int points) : Entity(health, points), damage(damage)
{
	if (health<=0)
	{
		throw std::invalid_argument("The health and must be positive");
	}

	if (damage<0)
	{
		throw std::invalid_argument("The damage and must be non-negative");
	}

	if (points<0)
	{
		throw std::invalid_argument("The points and must be non-negative");
	}
}

int Enemy::getDamage()const
{
	return damage;
}
void Enemy::setDamage(int value){damage = value;}
void Enemy::showStats() const
{
	std::cout << std::format("Health: {}. Damage: {}. Reward: {}", health, damage, points) << std::endl;
}

Enemy::Enemy(const Enemy& other) : Entity(other), damage(other.damage){}
Enemy& Enemy::operator=(const Enemy& other)
{
	if (this != &other) 
    {
        Entity::operator=(other);
        damage = other.damage;
    }
    return *this;
}

std::ostream& operator << (std::ostream &os, const Enemy &enemy)
{
	os << enemy.getHealth() << " " << enemy.getDamage() << " " << enemy.getPoints() << " ";
	return os;
}

std::istream& operator >> (std::istream &in, Enemy& enemy)
{
	int health;
	int damage;
	int reward;
	in >> health >> damage >> reward;
	if (!in.good()) throw std::runtime_error("Failed to read enemy parameters");
	if(health <= 0 or damage <= 0 or reward < 0)
	{
		throw std::invalid_argument("Wrong arguments for Enemy!");
	}
	enemy.setHealth(health);
	enemy.setDamage(damage);
	enemy.setPoints(reward);
	return in;
}
