#include "hand.hpp"

Hand::Hand(size_t size) : max_size(size){}

Hand::~Hand(){
    for(Spell* spell : spells){
        delete spell;
    }
    spells.clear();
}

bool Hand::addSpell(Spell* spell){
    if(spells.size() < max_size){
        spells.push_back(spell);
        return true;
    }
    delete spell;
    return false;
}

void Hand::useSpell(size_t index){
    if(index < spells.size()){
        delete spells[index];
        spells.erase(spells.begin() + index);
    }
}

Spell* Hand::getSpell(size_t index) const{
    if(index < spells.size()){
        return spells[index];
    }
    return nullptr;
}

const std::vector<Spell*>& Hand::getSpells() const{
    return spells;
}

size_t Hand::getSpellCount() const{
    return spells.size();
}

bool Hand::isFull() const{
    return spells.size() >= max_size;
}