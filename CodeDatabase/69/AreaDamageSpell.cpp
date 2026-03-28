#include "AreaDamageSpell.h"

AreaDamageSpell::AreaDamageSpell(std::string name, int damage, int areaSize)
    : name_(std::move(name)), damage_(damage), areaSizeX_(areaSize), areaSizeY_(areaSize)
{
}

std::string AreaDamageSpell::GetName() const
{
    return name_;
}

void AreaDamageSpell::ApplyBuffs(BuffState const& state)
{
    areaSizeX_ += state.areaSizeBonus;
    areaSizeY_ += state.areaSizeBonus;
}

bool AreaDamageSpell::Apply(Player& caster, Game& game, Position target)
{
    if (!game.IsInBounds(target))
    {
        return false;
    }
    game.DamageArea(target, areaSizeX_, areaSizeY_, damage_);
    return true;
}