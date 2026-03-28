#include "castle.h"
#include "../abstractNinterfaces/defines.h"
#include "../map.h"
#include <windows.h>
#include <iostream>
void castle::environmentInteraction(map* map, coords futureMapPosition)
{
    if(map->getTiles()[futureMapPosition.y][futureMapPosition.x].name == NOTHING){
        std::cout << "Enemy burst out from the castle to (" << futureMapPosition.x << ", " << futureMapPosition.y << ")\n";
        map->getTiles()[futureMapPosition.y][futureMapPosition.x].name = ENEMY;
        srand(time(NULL));
        Sleep(1000);
        enemy* enemy = new class enemy(ENEMYHP, ENEMYAD);
        map->getTiles()[futureMapPosition.y][futureMapPosition.x].address = enemy;
        enemy->setMapPosition(futureMapPosition.x, futureMapPosition.y);
        enemy->setId(map->getEnemies().back()->getId()+1);
        map->getEnemies().push_back(enemy);
    }else{
        makeTurn(map);
    }
}

void castle::makeTurn(map* map)
{
    Sleep(1000);
    srand(time(NULL));
    coords futureMapPosition = this->getMapPosition();
    char random = rand()%5;
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
            std::cout << "Castle is empty this turn\n";
            break;
        }
}

void castle::setSpawnTimer(size_t spawnTimer)
{
    this->spawnTimer = spawnTimer;
}

size_t castle::getSpawnTimer()
{
    return this->spawnTimer;
}

castle::castle(int HP, size_t spawnTimer) : spawnTimer(spawnTimer)
{
    this->setHP(HP);
    this->setId(0);
    this->setMapPosition(0,0);
}

castle::castle(int HP) : castle(HP, STDSPAWNTIMER){}

castle::castle() : castle(CASTLEHP){}