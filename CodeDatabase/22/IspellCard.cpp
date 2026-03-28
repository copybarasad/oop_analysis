#include "spells/IspellCard.h"

std::pair<int, int> ISpellCard::use() { return {-1, -1} ;}

std::string ISpellCard::name() const { return ""; }

void ISpellCard::levelUp(){}

void ISpellCard::use(ISpellCard &spell) {}

int ISpellCard::getRadius(){return 0;}