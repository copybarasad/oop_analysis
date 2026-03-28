#include "trap-spell-card.hpp"
#include "field.hpp"
#include "player.hpp"

game::TrapSpellCard::TrapSpellCard(size_t spell_damage):
  SpellCard(spell_damage)
{}

void game::TrapSpellCard::cast(Field& field, Position pos)
{
  field.set_square_type(SquareType::Trap, pos, spell_damage_);
}
