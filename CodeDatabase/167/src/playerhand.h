#ifndef PLAYERHAND_H
#define PLAYERHAND_H

#include <vector>
#include <memory>
#include "spellcard.h"

class PlayerHand
{
private:
  std::vector<std::unique_ptr<SpellCard>> spells;
  int maxSize;
  int enemiesDefeated;

public:
  explicit PlayerHand(int maxHandSize);

  int getSize() const;
  int getMaxSize() const;
  int getEnemiesDefeated() const;
  bool isFull() const;
  bool isEmpty() const;

  bool addSpell(std::unique_ptr<SpellCard> spell);
  bool removeSpell(int index);
  void clear();

  const SpellCard *getSpell(int index) const;
  void useSpell(int index, Player &player, GameField &field,
                int targetX, int targetY);

  void enemyDefeated();
  bool buySpell(std::unique_ptr<SpellCard> spell, int &playerScore);
  void addRandomSpell();

  void display() const;

private:
  std::unique_ptr<SpellCard> createRandomSpell() const;
};

#endif