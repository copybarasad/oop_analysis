#include "aoespell.h"
#include "player.h"
#include "gamefield.h"
#include "enemy.h"
#include <iostream>
#include <algorithm>

AoeSpell::AoeSpell(const std::string &spellName,
                   const std::string &spellDescription,
                   int cost, int spellRange, int spellDamage, int spellAreaSize)
    : SpellCard(spellName, spellDescription, cost, spellRange),
      damage(spellDamage), areaSize(spellAreaSize) {}

bool AoeSpell::canCast(const Player &player, const GameField &field,
                       int targetX, int targetY) const
{

  return isInRange(player, targetX, targetY);
}

void AoeSpell::cast(Player &player, GameField &field,
                    int targetX, int targetY) const
{
  if (!canCast(player, field, targetX, targetY))
  {
    std::cout << "Cannot cast " << name << " on this target!" << std::endl;
    return;
  }

  std::cout << name << " explodes at (" << targetX << ", " << targetY << ")!" << std::endl;
  applyAreaDamage(player, field, targetX, targetY);
}

std::unique_ptr<SpellCard> AoeSpell::clone() const
{
  return std::unique_ptr<SpellCard>(new AoeSpell(*this));
}

int AoeSpell::getDamage() const
{
  return damage;
}

int AoeSpell::getAreaSize() const
{
  return areaSize;
}

void AoeSpell::display() const
{
  std::cout << name << " (Cost: " << manaCost << ", Range: " << range
            << ", Damage: " << damage << ", Area: " << areaSize << "x" << areaSize << ")\n";
  std::cout << description << std::endl;
}

void AoeSpell::applyAreaDamage(Player &player, GameField &field,
                               int centerX, int centerY) const
{
  int halfSize = areaSize / 2;
  int enemiesHit = 0;

  for (int y = centerY - halfSize; y <= centerY + halfSize; y++)
  {
    for (int x = centerX - halfSize; x <= centerX + halfSize; x++)
    {
      if (field.getCellType(x, y) == CellType::ENEMY)
      {
        // Находим врага на этой клетке
        const auto &enemies = field.getEnemies();
        for (const auto &enemy : enemies)
        {
          if (enemy->getPosX() == x && enemy->getPosY() == y && enemy->isAlive())
          {
            enemy->takeDamage(damage);
            enemiesHit++;

            if (!enemy->isAlive())
            {
              player.addScore(30);
            }
          }
        }
      }
    }
  }

  if (enemiesHit > 0)
  {
    std::cout << "Hit " << enemiesHit << " enemies for " << damage << " damage each!" << std::endl;
  }
  else
  {
    std::cout << "The spell hit no enemies." << std::endl;
  }
}