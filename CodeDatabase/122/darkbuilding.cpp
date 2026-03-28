#include "darkbuilding.h"

DarkBuilding::DarkBuilding(int spawnInterval, int xPos, int yPos) : m_xPos(xPos), m_yPos(yPos), m_spawnInterval(spawnInterval) {}

void DarkBuilding::buildingUpdate()
{
    --m_spawnInterval;
}

bool DarkBuilding::readyToSpawn()
{
    if (!m_spawnInterval)
    {
        m_spawnInterval = BuildingsConfig::SpawnInterval;
        return true;
    }
    return false;
}
