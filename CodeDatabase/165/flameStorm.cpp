#include "flameStorm.h"
#include "../abstractNinterfaces/defines.h"
#include "../map.h"
#include "../human/hero.h"
#include <iostream>

std::vector<coords> flameStorm::targets(map *map, hero *hero)
{
    std::vector<coords> availableTargets;
    std::cout << "Write the difference between your position and a targeted one in a sequence \"x, y\":\n";
    int targetNumberX = hero->getANumber();
    int targetNumberY = hero->getANumber();
    while(hero->getMapPosition().x + targetNumberX < 0 || hero->getMapPosition().x + targetNumberX > map->getLength() || 
    hero->getMapPosition().y + targetNumberY < 0 || hero->getMapPosition().y + targetNumberY > map->getHeight()
    || targetNumberX > FLAMESTORMBLAST || targetNumberY > FLAMESTORMBLAST){
        std::cout << "Out of range. Write again \"x, y\":\n";
        targetNumberX = hero->getANumber();
        targetNumberY = hero->getANumber();
    }
    coords tmp;
    for(int y = hero->getMapPosition().y + targetNumberY; y < hero->getMapPosition().y + targetNumberY + FLAMESTORMBLAST; y++){
        for(int x = hero->getMapPosition().x + targetNumberX; x < hero->getMapPosition().x + targetNumberX + FLAMESTORMBLAST; x++){
            if(x >= 0 && x < map->getLength() && y>= 0 && y < map->getHeight()){
                tmp.x = x;
                tmp.y = y;
                availableTargets.push_back(tmp);
            }
        }
    }
    return availableTargets;
}

void flameStorm::use(map *map, hero *hero)
{
    if(hero->getMP() >= MANACOST*(0.9+MODIFICATOR*rank)){
        hero->setMP(hero->getMP()-MANACOST);
        enemy* enemy;
        for(coords x : this->targets(map, hero)){
            if(map->getTiles()[x.y][x.x].name == ENEMY){
                enemy = (class enemy*) map->getTiles()[x.y][x.x].address;
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
        }
        std::cout << "Only " << hero->getMP() << " MP left\n";
    }else{
        std::cout << "You lack the mana to cast a spell\n";
    }
    int j = 0;
    for(auto x : hero->getInventory().getSpellList()){
        if(x == this){
            hero->getInventory().getSpellList().erase(hero->getInventory().getSpellList().begin() + j);
        }
        j++;
    }
}

void flameStorm::setRank(size_t rank)
{
    if(rank){
        this->setBlastRange(this->getBlastRange()+rank-this->getRank());
    }
    this->rank = rank;
}

void flameStorm::setBlastRange(size_t blastRange)
{
    this->blastRange = blastRange;
}

size_t flameStorm::getBlastRange()
{
    return this->blastRange;
}

flameStorm::flameStorm(size_t rank)
{
    this->setDmg(FLAMESTORMDMG);
    this->setBlastRange(FLAMESTORMBLAST);
    this->setRange(FLAMESTORMRNG);
    this->setRank(rank);
    this->setName("Flamestorm");
}

flameStorm::flameStorm() : flameStorm(0){}
