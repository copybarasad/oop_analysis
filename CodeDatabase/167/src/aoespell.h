#ifndef AOESPELL_H
#define AOESPELL_H

#include "spellcard.h"
#include <memory>

class AoeSpell : public SpellCard
{
private:
  int damage;
  int areaSize;

public:
  AoeSpell(const std::string &spellName, const std::string &spellDescription,
           int cost, int spellRange, int spellDamage, int spellAreaSize);

  bool canCast(const Player &player, const GameField &field,
               int targetX, int targetY) const override;
  void cast(Player &player, GameField &field,
            int targetX, int targetY) const override;
  std::unique_ptr<SpellCard> clone() const override;

  int getDamage() const;
  int getAreaSize() const;

  void display() const override;

private:
  void applyAreaDamage(Player &player, GameField &field,
                       int centerX, int centerY) const;
};

#endif