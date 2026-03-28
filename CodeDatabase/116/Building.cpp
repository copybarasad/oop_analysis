#include "Building.h"

Building::Building(int x, int y,int Health, int frequency):Character(x,y,Health),spawnFrequency(frequency),moveCounter(0){}

bool Building::spawnIsReady(){
    if (spawnFrequency == moveCounter){moveCounter = 0; return true;}
    else{return false;}
}

void Building::updateCounter(){moveCounter += 1;};