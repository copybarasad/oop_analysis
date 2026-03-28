#include "directdamagespell.h"
#include "player.h"
#include "gamefield.h"
#include "enemy.h"
#include <iostream>

DirectDamageSpell::DirectDamageSpell(const std::string &spellName,
                                     const std::string &spellDescription,
                                     int cost, int spellRange, int spellDamage)
    : SpellCard(spellName, spellDescription, cost, spellRange), damage(spellDamage) {}

bool DirectDamageSpell::canCast(const Player &player, const GameField &field,
                                int targetX, int targetY) const
{

  if (!isInRange(player, targetX, targetY))
  {
    return false;
  }

  CellType cellType = field.getCellType(targetX, targetY);
  return cellType == CellType::ENEMY;
}

void DirectDamageSpell::cast(Player &player, GameField &field,
                             int targetX, int targetY) const
{
  if (!canCast(player, field, targetX, targetY))
  {
    std::cout << "Cannot cast " << name << " on this target!" << std::endl;
    return;
  }

  const auto &enemies = field.getEnemies();
  for (const auto &enemy : enemies)
  {
    if (enemy->getPosX() == targetX && enemy->getPosY() == targetY && enemy->isAlive())
    {
      enemy->takeDamage(damage);
      std::cout << name << " hit " << enemy->getType()
                << " for " << damage << " damage!" << std::endl;

      if (!enemy->isAlive())
      {
        player.addScore(50);
        std::cout << enemy->getType() << " defeated!" << std::endl;
      }
      return;
    }
  }
}

std::unique_ptr<SpellCard> DirectDamageSpell::clone() const
{
  return std::unique_ptr<SpellCard>(new DirectDamageSpell(*this));
}

int DirectDamageSpell::getDamage() const
{
  return damage;
}

void DirectDamageSpell::display() const
{
  std::cout << name << " (Cost: " << manaCost << ", Range: " << range
            << ", Damage: " << damage << ")\n";
  std::cout << description << std::endl;
}