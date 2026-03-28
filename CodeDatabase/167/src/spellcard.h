#ifndef SPELLCARD_H
#define SPELLCARD_H

#include <string>
#include <memory>

class Player;
class GameField;
class Enemy;

class SpellCard
{
protected:
  std::string name;
  std::string description;
  int manaCost;
  int range;

public:
  SpellCard(const std::string &spellName, const std::string &spellDescription,
            int cost, int spellRange);
  virtual ~SpellCard() = default;

  std::string getName() const;
  std::string getDescription() const;
  int getManaCost() const;
  int getRange() const;

  virtual bool canCast(const Player &player, const GameField &field,
                       int targetX, int targetY) const = 0;
  virtual void cast(Player &player, GameField &field,
                    int targetX, int targetY) const = 0;
  virtual std::unique_ptr<SpellCard> clone() const = 0;

  bool isInRange(const Player &player, int targetX, int targetY) const;
  virtual void display() const;
};

#endif