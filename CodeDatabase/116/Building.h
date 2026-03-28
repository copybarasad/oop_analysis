#pragma once

#include "Character.h"

class Building : public Character {
private:
    const int spawnFrequency;
    int moveCounter;
    
public:
    Building(int x = 0, int y = 0,int Health = 40, int frequency = 5);
    ~Building() = default;
    bool spawnIsReady();
    void updateCounter();
    int getAttackFrequency() const { return spawnFrequency; }
};