#include "player.h"
#include "enemy.h"
#include "gamefield.h"
#include <iostream>
#include <algorithm>

Player::Player(const std::string &playerName, int initialHealth, int playerDamage, int handSize)
    : name(playerName), health(initialHealth), maxHealth(initialHealth),
      damage(playerDamage), score(0), posX(0), posY(0), hand(handSize) {}

int Player::getHealth() const
{
  return health;
}

int Player::getMaxHealth() const
{
  return maxHealth;
}

int Player::getDamage() const
{
  return damage;
}

int Player::getScore() const
{
  return score;
}

std::string Player::getName() const
{
  return name;
}

int Player::getPosX() const
{
  return posX;
}

int Player::getPosY() const
{
  return posY;
}

bool Player::isAlive() const
{
  return health > 0;
}

PlayerHand &Player::getHand()
{
  return hand;
}

const PlayerHand &Player::getHand() const
{
  return hand;
}

void Player::setHealth(int newHealth)
{
  health = newHealth;
  if (health < 0)
    health = 0;
  if (health > maxHealth)
    health = maxHealth;
}

void Player::setMaxHealth(int newMaxHealth)
{
  maxHealth = newMaxHealth;
  if (health > maxHealth)
    health = maxHealth;
}

void Player::setDamage(int newDamage)
{
  damage = newDamage;
}

void Player::setScore(int newScore)
{
  score = newScore;
}

void Player::setPosition(int x, int y)
{
  posX = x;
  posY = y;
}

void Player::takeDamage(int damageAmount)
{
  int oldHealth = health;
  health -= damageAmount;
  if (health < 0)
  {
    health = 0;
  }

  std::cout << name << " takes " << damageAmount << " damage! ";
  std::cout << "Health: " << oldHealth << " -> " << health << std::endl;
}

void Player::addScore(int points)
{
  score += points;
}

void Player::heal(int amount)
{
  int oldHealth = health;
  health += amount;
  if (health > maxHealth)
  {
    health = maxHealth;
  }

  std::cout << name << " heals for " << amount << " health! ";
  std::cout << "Health: " << oldHealth << " -> " << health << std::endl;
}

void Player::attack(Enemy &enemy) const
{
  std::cout << name << " attacks " << enemy.getType()
            << " for " << damage << " damage!" << std::endl;
  enemy.takeDamage(damage);
}

void Player::move(int newX, int newY)
{
  posX = newX;
  posY = newY;
}

void Player::castSpell(int spellIndex, GameField &field, int targetX, int targetY)
{
  hand.useSpell(spellIndex, *this, field, targetX, targetY);
}

void Player::enemyDefeated()
{
  hand.enemyDefeated();
}