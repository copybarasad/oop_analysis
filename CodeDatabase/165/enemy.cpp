#include "enemy.h"
#include "../abstractNinterfaces/defines.h"
#include "../map.h"
#include <iostream>
#include <windows.h>
void enemy::environmentInteraction(map * map, coords futureMapPosition)
{   
    char tileName = map->getTiles()[futureMapPosition.y][futureMapPosition.x].name;
    switch(tileName){
        case NOTHING:
            std::cout << "Enemy moved from (" << this->getMapPosition().x << ", " << this->getMapPosition().y << ") ";
            map->getTiles()[this->getMapPosition().y][this->getMapPosition().x].name = NOTHING;
            map->getTiles()[futureMapPosition.y][futureMapPosition.x].name = ENEMY;
            map->getTiles()[futureMapPosition.y][futureMapPosition.x].address = this;
            this->setMapPosition(futureMapPosition.x, futureMapPosition.y);
            std::cout << "to (" << this->getMapPosition().x << ", " << this->getMapPosition().y << ")\n";
            break;
        case HERO:
            {
            hero* hero;
            hero = map->getHero();
            hero->setHP(hero->getHP()-this->getAttackDmg());
            std::cout << "Enemy attacks the Hero. Only " << hero->getHP() <<"HP remain.\n";
            if(hero->getHP() <= 0){
                map->getTiles()[this->getMapPosition().y][this->getMapPosition().x].name = 'X';
                std::cout << "You died\n";
            }
            break;
            }
        case CASTLE:
            {
            castle* castle;
            castle = (class castle*) map->getTiles()[futureMapPosition.y][futureMapPosition.x].address;
            castle->setHP(castle->getHP()+this->getAttackDmg());
            std::cout << "Enemy repairs their Castle at (" << castle->getMapPosition().x << ", " 
            << castle->getMapPosition().y << ")\n";
            break;
            }
        default:
            this->makeTurn(map);
    }
}

void enemy::makeTurn(map* map)
{
    Sleep(1000);
    srand(time(NULL));
    coords futureMapPosition = this->getMapPosition();
    char random;
    random = rand()%5;
    switch(random){
        case 0:
            if(futureMapPosition.y){
                futureMapPosition.y--;
                this->environmentInteraction(map, futureMapPosition);
            }else{
                this->makeTurn(map);
            }
            break;
        case 1:
            if(futureMapPosition.x){
                futureMapPosition.x--;
                this->environmentInteraction(map, futureMapPosition);
            }else{
                this->makeTurn(map);
            }
            break;
        case 2:
            if(futureMapPosition.y + 1 != map->getHeight()){
                futureMapPosition.y++;
                this->environmentInteraction(map, futureMapPosition);
            }else{
                this->makeTurn(map);
            }
            break;
        case 3:
            if(futureMapPosition.x + 1 != map->getLength()){
                futureMapPosition.x++;
                this->environmentInteraction(map, futureMapPosition);
            }else{
                this->makeTurn(map);
            }
            break;
        default:
            std::cout << "Enemy skips its turn\n";
            break;
    }
}

enemy::enemy(int HP, size_t attackDmg)
{
    this->setHP(HP);
    this->setAttackDmg(attackDmg);
    this->setMapPosition(0,0);
    this->setId(0);
}

enemy::enemy(int HP) : enemy(HP, ENEMYAD){}

enemy::enemy() : enemy(ENEMYHP){}