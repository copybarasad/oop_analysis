#ifndef AREA_DAMAGE_SPELL_CARD_HPP
#define AREA_DAMAGE_SPELL_CARD_HPP

#include "spell-card.hpp"

namespace game
{
  constexpr size_t INITIAL_DAMAGE_RANGE = 2;

  class AreaDamageSpellCard final: public SpellCard
  {
  public:
    AreaDamageSpellCard(size_t spell_damage = INITIAL_DAMAGE_BOOST);

    void cast(Field& field, Position pos) override;

  private:
    size_t damage_range_ = INITIAL_DAMAGE_RANGE;
  };
}

#endif
