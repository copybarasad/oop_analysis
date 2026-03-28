#include "BuffSpell.h"


BuffSpell::BuffSpell(std::string name) 
    : name_(std::move(name)) 
{}

std::string BuffSpell::GetName() const 
{ 
    return name_; 
}

bool BuffSpell::Apply(Player& caster, Game& game, Position target)
{
    BuffState buff;
    buff.damageRangeBonus = 1;
    buff.areaSizeBonus    = 1;
    buff.trapDamageBonus  = 2;
    buff.summonCountBonus = 1;

    caster.AddBuff(buff);

    std::cout << "Buff accumulated! It will enhance your next spell.\n";
    return true;
}