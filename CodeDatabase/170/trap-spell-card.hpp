#ifndef TRAP_SPELL_CARD_HPP
#define TRAP_SPELL_CARD_HPP

#include "spell-card.hpp"

namespace game
{
  class TrapSpellCard final: public SpellCard
  {
  public:
    TrapSpellCard(size_t spell_damage = INITIAL_DAMAGE_BOOST);

    void cast(Field& field, Position pos) override;
  };
}

#endif
