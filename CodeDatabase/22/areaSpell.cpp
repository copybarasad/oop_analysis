#include "spells/areaSpell.h"

AreaSpell::AreaSpell(std::string name, std::string description, int damage, int radius):
    name_(name), description_(description), damage_(damage), radius_(radius), level_(1){}

// Active methods
std::pair<int, int> AreaSpell::use()
{
    std::pair<int, int> result = {damage_, radius_};
    return result;
}


void AreaSpell::levelUp(){
    radius_+=2;
    level_++;
}

std::string AreaSpell::name() const {
    return name_;
}

int AreaSpell::getRadius() {return radius_;}

void AreaSpell::use(ISpellCard &spell){}