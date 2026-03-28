#include "spells/improveSpell.h"
#include "spells/IspellCard.h"


ImproveSpell::ImproveSpell(){}

void ImproveSpell::use(ISpellCard& spell){
    spell.levelUp();
}

std::string ImproveSpell::name() const {
    return "Заклинание улудшения";
}

void ImproveSpell::levelUp(){}

std::pair<int, int> ImproveSpell::use() {return {0,0};}
int ImproveSpell::getRadius() {return 0;}