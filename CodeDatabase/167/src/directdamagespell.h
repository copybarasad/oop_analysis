#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "spellcard.h"
#include <memory>

class DirectDamageSpell : public SpellCard
{
private:
  int damage;

public:
  DirectDamageSpell(const std::string &spellName, const std::string &spellDescription,
                    int cost, int spellRange, int spellDamage);

  bool canCast(const Player &player, const GameField &field,
               int targetX, int targetY) const override;
  void cast(Player &player, GameField &field,
            int targetX, int targetY) const override;
  std::unique_ptr<SpellCard> clone() const override;

  int getDamage() const;

  void display() const override;
};

#endif