#include "spellcard.h"
#include "player.h"
#include "gamefield.h"
#include <iostream>
#include <cmath>

SpellCard::SpellCard(const std::string &spellName, const std::string &spellDescription,
                     int cost, int spellRange)
    : name(spellName), description(spellDescription), manaCost(cost), range(spellRange) {}

std::string SpellCard::getName() const
{
  return name;
}

std::string SpellCard::getDescription() const
{
  return description;
}

int SpellCard::getManaCost() const
{
  return manaCost;
}

int SpellCard::getRange() const
{
  return range;
}

bool SpellCard::isInRange(const Player &player, int targetX, int targetY) const
{
  int playerX = player.getPosX();
  int playerY = player.getPosY();
  int distance = std::abs(playerX - targetX) + std::abs(playerY - targetY);
  return distance <= range;
}

void SpellCard::display() const
{
  std::cout << name << " (Cost: " << manaCost << ", Range: " << range << ")\n";
  std::cout << description << std::endl;
}