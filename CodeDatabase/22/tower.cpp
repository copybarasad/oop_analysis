#include "characters/tower.h"
#include "field/position.h"


Tower::Tower(int health, Position position): Character(health, 0, position){
    spell_ = std::make_unique<DirectSpell>("Огненный шар", "", 10, 2);
}

DirectSpell* Tower::attack(){
    return spell_.get();
}