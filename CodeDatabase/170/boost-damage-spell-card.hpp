#ifndef BOOST_DAMAGE_SPELL_CARD_HPP
#define BOOST_DAMAGE_SPELL_CARD_HPP

#include "spell-card.hpp"

namespace game
{
  class BoostDamageSpellCard final: public SpellCard
  {
  public:
    BoostDamageSpellCard(size_t spell_damage = INITIAL_DAMAGE_BOOST);

    void cast(Field& field, Position pos) override;
  };
}

#endif
