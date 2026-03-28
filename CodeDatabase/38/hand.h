#pragma once

#include "areadamage.h"
#include "directdamage.h"
#include "field.h"
#include "spell.h"

#include <iostream>
#include <map>
#include <random>
#include <stdexcept>
#include <vector>
class Enemies;
class Hand {
private:
  std::map<int, int> hand;
  unsigned int max_amount;

public:
  unsigned int get_max() const;
  unsigned int get_amount() const;
  const std::map<int, int> &showHand() const;
  bool useSpell(Spell::SpellType type, std::string &persistent_str,
                DirectDamage &dird, AreaDamage &ad, Player *player,
                Enemies &enemies, Field *field);
  void addCard(Spell::SpellType type);
  unsigned int getCardAmount(Spell::SpellType type);
  void DrawHand();
  Hand(unsigned int max);
};
