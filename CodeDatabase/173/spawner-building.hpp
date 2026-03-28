#ifndef SPAWNER_BUILDING_HPP
#define SPAWNER_BUILDING_HPP

#define SPAWNER_BUILDING_BYTE 1

#include "../enemy.hpp"
#include "generic-building.hpp"

class SpawnerBuilding : public GenericBuilding {
  public:
    SpawnerBuilding(int hp);
    SpawnerBuilding(const SpawnerBuilding &building);
    ~SpawnerBuilding();
    Building *clone() override;

    void setEnemy(const Enemy *enemy);
    void setSpawnRate(int spawnRate);
    void update() override;
    void setMap(Map *map) override;

    std::string chr() override;
    Color color() override;

    bytedump dump() override;
    SpawnerBuilding(bytedump bytes);

  protected:
    Enemy *m_Enemy;
    int m_SpawnRate;
    int m_TickCount;
    Map *m_Map;
};

#endif /* SPAWNER_BUILDING_HPP */
