#include "enemy.h"
#include "player.h"
#include <iostream>
#include <algorithm>

int Enemy::nextId = 0;

Enemy::Enemy(const std::string &enemyType, int initialHealth, int enemyDamage)
    : type(enemyType), health(initialHealth), maxHealth(initialHealth),
      damage(enemyDamage), posX(0), posY(0), id(nextId++) {}

int Enemy::getHealth() const
{
  return health;
}

int Enemy::getMaxHealth() const
{
  return maxHealth;
}

int Enemy::getDamage() const
{
  return damage;
}

std::string Enemy::getType() const
{
  return type;
}

int Enemy::getPosX() const
{
  return posX;
}

int Enemy::getPosY() const
{
  return posY;
}

int Enemy::getId() const
{
  return id;
}

bool Enemy::isAlive() const
{
  return health > 0;
}

// Сеттеры для загрузки игры
void Enemy::setHealth(int newHealth)
{
  health = newHealth;
  if (health < 0)
    health = 0;
  if (health > maxHealth)
    health = maxHealth;
}

void Enemy::setMaxHealth(int newMaxHealth)
{
  maxHealth = newMaxHealth;
  if (health > maxHealth)
    health = maxHealth;
}

void Enemy::setDamage(int newDamage)
{
  damage = newDamage;
}

void Enemy::setPosition(int x, int y)
{
  posX = x;
  posY = y;
}

void Enemy::setId(int newId)
{
  id = newId;
  if (newId >= nextId)
  {
    nextId = newId + 1;
  }
}

void Enemy::takeDamage(int damageAmount)
{
  int oldHealth = health;
  health -= damageAmount;
  if (health < 0)
  {
    health = 0;
  }

  std::cout << type << " (ID: " << id << ") takes " << damageAmount << " damage! ";
  std::cout << "Health: " << oldHealth << " -> " << health;

  if (!isAlive())
  {
    std::cout << " - DEFEATED!";
  }
  std::cout << std::endl;
}

void Enemy::heal(int amount)
{
  int oldHealth = health;
  health += amount;
  if (health > maxHealth)
  {
    health = maxHealth;
  }

  std::cout << type << " heals for " << amount << " health! ";
  std::cout << "Health: " << oldHealth << " -> " << health << std::endl;
}

void Enemy::attack(Player &player) const
{
  std::cout << type << " (ID: " << id << ") attacks " << player.getName()
            << " for " << damage << " damage!" << std::endl;
  player.takeDamage(damage);
}

void Enemy::move(int newX, int newY)
{
  posX = newX;
  posY = newY;
}