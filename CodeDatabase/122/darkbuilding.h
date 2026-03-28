#ifndef DARKBUILDING_H
#define DARKBUILDING_H

#include <iostream>

#include "constants.h"

class DarkBuilding
{
public:
    DarkBuilding(int spawnInterval=2, int xPos=BoardConfig::Size-2, int yPos=0);
    int getXPos() const { return m_xPos; }
    int getYPos() const { return m_yPos; }
    int& getSpawnInterval() { return m_spawnInterval; }
    void buildingUpdate();
    bool readyToSpawn();
private:
    int m_xPos, m_yPos;
    int m_spawnInterval;
};

#endif // DARKBUILDING_H
