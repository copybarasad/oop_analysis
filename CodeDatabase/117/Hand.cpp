#include "Hand.h"

#include "../libs/json.hpp"
#include <map>

Hand::Hand(int size): size(size), count(0), upgradePower(0) {}

Hand::Hand(): Hand(DEFAULT_HAND_SIZE) {}

Hand::~Hand(){
    while(this->count != 0){
        this->removeSpell(this->count - 1);
    }
}

void Hand::upgradeNextSpell(int power){
    this->upgradePower += power;
}

int Hand::getUpgradePower(){
    return this->upgradePower;
}

void Hand::setUpgradePower(int upgradePower){
    this->upgradePower = upgradePower;
}

void Hand::setSize(int size){
    this->size = size;
}

int Hand::getSize(){
    return this->size;
}

int Hand::getCount(){
    return this->count;
}

bool Hand::addSpell(SpellType spell){
    if(this->count >= this->size){
        return false;
    }
    ++(this->count);
    this->spells.push_back(spell);
    return true;
}

bool Hand::addRandomSpell(){
    std::srand(std::time(0));
    int randNum = rand() % 5;
    return this->addSpell((SpellType)randNum);
}

void Hand::removeSpell(int index){
    if(this->count <= index || this->count == 0){
        return;
    }
    this->spells.erase(this->spells.begin() + index);
    --(this->count);
}

SpellType Hand::getSpell(int index){
    if(index > this->count){
        throw std::out_of_range("Index of hand out of range");
    }
    return this->spells[index];
}

void Hand::clear(){
    this->count = 0;
    this->spells.clear();
}

void Hand::displayHand(){
    std::map<SpellType, std::string> spellTypeToString = {
        {SpellType::DirDMGSpell, "Direct damage spell"},
        {SpellType::AreaDMGSpell, "Area damage spell"},
        {SpellType::SummonSpell, "Summon spell"},
        {SpellType::TrapSpell, "Trap spell"},
        {SpellType::UpgradeSpell, "Upgrade spell"},
    };

    if(this->count == 0){
        std::cout << "Hand is empty\n";
        return;
    }
    
    std::cout << "Hand have: ";
    for(int i = 0; i < this->count - 1; ++i){
        std::cout << spellTypeToString[this->spells[i]] << ", ";
    }
    std::cout << spellTypeToString[this->spells[this->count - 1]] << '\n';
}