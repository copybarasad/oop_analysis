#include "iceSpear.h"
#include "../map.h"
#include "../human/hero.h"
#include "../abstractNinterfaces/defines.h"
#include <iostream>

std::vector<coords> iceSpear::targets(map *map, hero *hero)
{
    std::vector<coords> availableTargets;
    for(enemy* x : map->getEnemies()){
        if(abs(x->getMapPosition().x - hero->getMapPosition().x) < this->getRange() &&
        abs(x->getMapPosition().y - hero->getMapPosition().y) < this->getRange()){
            availableTargets.push_back(x->getMapPosition());
            std::cout << availableTargets.size() << ". Enemy at (" << x->getMapPosition().x << ", " << x->getMapPosition().y << ")\n";
        }
    }
    if(availableTargets.empty()){
        std::cout << "No targets available\n";
    }
    return availableTargets;
}

void iceSpear::use(map *map, hero *hero)
{   
    if(hero->getMP() >= MANACOST*(0.9+MODIFICATOR*rank)){
        hero->setMP(hero->getMP()-MANACOST);
        std::vector<coords> availableTargets = this->targets(map, hero);
        if(!availableTargets.empty()){
            size_t targetNumber = hero->getANumber()-1;
            if(targetNumber >= 0 && targetNumber < availableTargets.size()){
                enemy* enemy = (class enemy*) map->getTiles()[availableTargets.at(targetNumber).y][availableTargets.at(targetNumber).x].address;
                if(enemy->getHP()-this->getDmg() > 0){
                    enemy->setHP(enemy->getHP()-this->getDmg());
                    std::cout << "You attack the Enemy. Now it has " << enemy->getHP() << "HP\n";
                }else{
                    class enemy* tmp;
                    map->getTiles()[enemy->getMapPosition().y][enemy->getMapPosition().x].name = NOTHING;
                    for(std::vector<class enemy*>::iterator it = map->getEnemies().begin() + enemy->getId() + 1; it < map->getEnemies().end(); it++){
                        tmp = *it;
                        tmp->setId(tmp->getId()-1);
                    }
                    map->getEnemies().erase(map->getEnemies().begin() + enemy->getId());
                    delete enemy;
                    std::cout << "You defeated the Enemy\n";
                }
            }
            int j = 0;
            for(auto x : hero->getInventory().getSpellList()){
                if(x == this){
                    hero->getInventory().getSpellList().erase(hero->getInventory().getSpellList().begin() + j);
                }
                j++;
            }
            std::cout << "Only " << hero->getMP() << " MP left\n";
        }else{
            hero->makeTurn(map);
        }
    }else{
        std::cout << "You lack the mana to cast a spell\n";
    }
}

void iceSpear::setRank(size_t rank)
{
    if(rank){
        this->setRange(this->getRange()+rank-this->getRank());
    }
    this->rank = rank;
}

iceSpear::iceSpear(size_t rank)
{
    this->setDmg(ICESPEARDMG);
    this->setRange(ICESPEARRNG);
    this->setRank(rank);
    this->setName("Ice Spear");
}

iceSpear::iceSpear() : iceSpear(0){}
