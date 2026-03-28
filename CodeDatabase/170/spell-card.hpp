#ifndef SPELL_CARD_HPP
#define SPELL_CARD_HPP

#include "base-types.hpp"

namespace game
{
  class Player;
  class Field;

  constexpr size_t INITIAL_DAMAGE_BOOST = 3;

  class SpellCard
  {
  public:
    virtual void cast(Field& field, Position pos) = 0;

  protected:
    size_t spell_damage_;
    explicit SpellCard(size_t spell_damage);
  };
}

#endif
