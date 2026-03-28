#include "spells/directSpell.h"

DirectSpell::DirectSpell(std::string name, std::string description, int damage, int radius):
    name_(name), desctription_(description), damage_(damage), radius_(radius), level_(1){}

// Active methods

std::pair<int, int> DirectSpell::use ()
{
    std::pair<int, int> result = {damage_, radius_};
    return result;
}

std::string DirectSpell::name() const {
    return name_;
}

void DirectSpell::levelUp(){
    damage_+=10;
    level_++;
}

int DirectSpell::getRadius() { return radius_; }

void DirectSpell::use(ISpellCard &spell){}