#include "SpellShop.h"
#include "fireball.h"
#include "BOMB.h"

SpellShop::SpellShop(){
    allSpells.push_back(std::make_unique<Fireball>(1, "Fireball", 50, 5, 2));
    allSpells.push_back(std::make_unique<BOMB>(2, "BOMB", 100, 10, 1));
    allSpells.push_back(std::make_unique<BOMB>(3, "Trap", 150, 50, 1));

}

const Spell* SpellShop::getSpell(size_t index) const {
    if(index < allSpells.size()){
        return allSpells[index].get();
    }
    return nullptr;
}

size_t SpellShop::size() const{
    return allSpells.size();
}