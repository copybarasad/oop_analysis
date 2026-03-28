#include "playerhand.h"
#include "directdamagespell.h"
#include "aoespell.h"
#include "player.h"
#include "gamefield.h"
#include <iostream>
#include <random>
#include <algorithm>

PlayerHand::PlayerHand(int maxHandSize)
    : maxSize(std::max(1, maxHandSize)), enemiesDefeated(0)
{

  addRandomSpell();
}

int PlayerHand::getSize() const
{
  return spells.size();
}

int PlayerHand::getMaxSize() const
{
  return maxSize;
}

int PlayerHand::getEnemiesDefeated() const
{
  return enemiesDefeated;
}

bool PlayerHand::isFull() const
{
  return spells.size() >= maxSize;
}

bool PlayerHand::isEmpty() const
{
  return spells.empty();
}

bool PlayerHand::addSpell(std::unique_ptr<SpellCard> spell)
{
  if (isFull())
  {
    std::cout << "Hand is full! Cannot add more spells." << std::endl;
    return false;
  }

  spells.push_back(std::move(spell));
  return true;
}

bool PlayerHand::removeSpell(int index)
{
  if (index < 0 || index >= static_cast<int>(spells.size()))
  {
    return false;
  }

  spells.erase(spells.begin() + index);
  return true;
}

void PlayerHand::clear()
{
  spells.clear();
}

const SpellCard *PlayerHand::getSpell(int index) const
{
  if (index < 0 || index >= static_cast<int>(spells.size()))
  {
    return nullptr;
  }

  return spells[index].get();
}

void PlayerHand::useSpell(int index, Player &player, GameField &field,
                          int targetX, int targetY)
{
  if (index < 0 || index >= static_cast<int>(spells.size()))
  {
    std::cout << "Invalid spell index!" << std::endl;
    return;
  }

  spells[index]->cast(player, field, targetX, targetY);
}

void PlayerHand::enemyDefeated()
{
  enemiesDefeated++;

  if (enemiesDefeated % 3 == 0 && !isFull())
  {
    std::cout << "You've earned a new spell for defeating "
              << enemiesDefeated << " enemies!" << std::endl;
    addRandomSpell();
  }
}

bool PlayerHand::buySpell(std::unique_ptr<SpellCard> spell, int &playerScore)
{
  if (isFull())
  {
    std::cout << "Hand is full! Cannot buy more spells." << std::endl;
    return false;
  }

  int spellCost = spell->getManaCost() * 10;
  if (playerScore >= spellCost)
  {
    playerScore -= spellCost;
    spells.push_back(std::move(spell));
    std::cout << "Spell purchased for " << spellCost << " points!" << std::endl;
    return true;
  }

  std::cout << "Not enough points! Need " << spellCost
            << ", but have " << playerScore << std::endl;
  return false;
}

void PlayerHand::addRandomSpell()
{
  if (isFull())
  {
    return;
  }

  spells.push_back(createRandomSpell());
}

void PlayerHand::display() const
{
  std::cout << "Spell Hand (" << spells.size() << "/" << maxSize << "):" << std::endl;
  for (size_t i = 0; i < spells.size(); ++i)
  {
    std::cout << i + 1 << ". ";
    spells[i]->display();
    std::cout << std::endl;
  }
}

std::unique_ptr<SpellCard> PlayerHand::createRandomSpell() const
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> spellTypeDist(0, 1);
  static std::uniform_int_distribution<> damageDist(15, 40);
  static std::uniform_int_distribution<> rangeDist(3, 6);
  static std::uniform_int_distribution<> costDist(2, 5);

  int spellType = spellTypeDist(gen);
  int damage = damageDist(gen);
  int range = rangeDist(gen);
  int cost = costDist(gen);

  if (spellType == 0)
  {

    std::string names[] = {"Firebolt", "Ice Lance", "Lightning Strike", "Shadow Bolt"};
    std::string descriptions[] = {
        "Shoots a bolt of fire at a single target.",
        "Launches a sharp icicle that pierces the enemy.",
        "Calls down a lightning strike on the target.",
        "Hurls a bolt of shadow energy."};

    static std::uniform_int_distribution<> nameDist(0, 3);
    int nameIndex = nameDist(gen);

    return std::unique_ptr<SpellCard>(
        new DirectDamageSpell(names[nameIndex], descriptions[nameIndex],
                              cost, range, damage));
  }
  else
  {

    std::string names[] = {"Fireball", "Frost Nova", "Chain Lightning", "Death Cloud"};
    std::string descriptions[] = {
        "Creates an explosion of fire that damages all nearby enemies.",
        "Releases a burst of frost that freezes everything around.",
        "Lightning jumps between multiple targets.",
        "Creates a toxic cloud that poisons the area."};

    static std::uniform_int_distribution<> nameDist(0, 3);
    int nameIndex = nameDist(gen);

    return std::unique_ptr<SpellCard>(
        new AoeSpell(names[nameIndex], descriptions[nameIndex],
                     cost, range, damage, 2));
  }
}