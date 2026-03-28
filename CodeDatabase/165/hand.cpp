#include "hand.h"
#include "../abstractNinterfaces/defines.h"
#include "./flameStorm.h"
#include "./iceSpear.h"
#include "../human/hero.h"
#include <iostream>
#include <windows.h>

hand::hand()
{
    this->setSpellListLimit(6);
    srand(time(NULL));
    size_t random = rand()%2;
    switch(random){
        case 0:
            this->getSpellList().push_back(new flameStorm(0));
            break;
        case 1:
            this->getSpellList().push_back(new iceSpear(0));
            break;
    }
}

std::vector<spell *>& hand::getSpellList()
{
    return this->spellList;
}

size_t hand::getSpellListLimit()
{
    return this->spellListLimit;
}

void hand::setSpellListLimit(size_t newLimit)
{
    this->spellListLimit = newLimit;
}

bool hand::craft(hero* hero)
{
    hero->setMP(hero->getMP()-MANACOST);
    std::cout << "You can craft:\n1. Ice Spear\n2. Flamestorm\n";
    size_t choice = hero->getANumber();
    return addSpell(choice, hero->getMagicalMastery());
}

bool hand::addSpell(size_t choice, size_t rank)
{
    switch(choice){
        case 1:
            this->getSpellList().push_back(new iceSpear(rank));
            break;
        case 2:
            this->getSpellList().push_back(new flameStorm(rank));
            break;
        default:
            std::cout << "You closed craft\n";
            return 1;
    }
    return 0;
}

void hand::freeHand(size_t index)
{
    if(this->getSpellList().size() > index){
        delete this->getSpellList().at(index);
        this->getSpellList().erase(this->getSpellList().begin()+index);
    }
}
