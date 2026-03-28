#include "spell.hpp"

Spell::Spell(SpellType sp_type) 
    : type(sp_type) {
    //improvment = 0;
}

SpellType Spell::getSpellType() {
    return type;
}

/*void Spell::improve() {
    improvment += 1;
}*/