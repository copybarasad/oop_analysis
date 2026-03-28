#include "spells/trapSpell.h"
#include "field/position.h"

TrapSpell::TrapSpell(int damage, Position position): damage_(damage), position_(position) {}


Position TrapSpell::position(){
    return position_;
}

std::pair<int, int> TrapSpell::use(){
    return {damage_, 0};
}

std::string TrapSpell::name() const {
    return "Ловушка";
}
void TrapSpell::levelUp() {
    damage_+=10;
}

void TrapSpell::use(ISpellCard &spell){}

int TrapSpell::getRadius(){return 1;}