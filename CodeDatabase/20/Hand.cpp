#include "../include/Hand.h"
#include <iostream>

//constructor
Hand::Hand(){
    this->capacity = 5;
    add_random_spell();
}

void Hand::add_random_spell() {
    if (spells.size() < capacity) {
        auto spell = SpellFactory::createRandomSpell();
        std::cout << "Added spell: " << spell->getName() << std::endl;
        spells.push(spell);
    } else {
        std::cout << "Your hand is full!" << std::endl;
    }
}



std::shared_ptr<Spell> Hand::get_spell(){
    if (spells.empty()) {
        return nullptr;
    }
    auto spell = spells.top();
    spells.pop();
    return spell;
    
}