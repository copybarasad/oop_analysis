#include "TrapSpell.h"


TrapSpell::TrapSpell(std::string name, int damage)
    : name_(std::move(name))
    , damage_(damage)
{}

std::string TrapSpell::GetName() const
{ 
    return name_; 
}

void TrapSpell::ApplyBuffs(BuffState const& state)
{
	damage_ += state.trapDamageBonus;
}

bool TrapSpell::Apply(Player& caster, Game& game, Position target)
{
    if (!game.IsInBounds(target)) {
        return false;
    }
    return game.PlaceTrap(target, name_, damage_);
}