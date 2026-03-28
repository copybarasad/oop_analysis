#include "spells/callSpell.h"

CallSpell::CallSpell() : level_(1), radius_(2){}

std::pair<int, int> CallSpell::use(){
    return {level_, radius_};
}

std::string CallSpell::name() const {
    return "Заклинание призыва";
}

void CallSpell::levelUp(){
    level_++;
}

int CallSpell::getRadius() { return radius_; }

void CallSpell::use(ISpellCard &spell){}