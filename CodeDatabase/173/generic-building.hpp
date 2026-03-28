#ifndef GENERIC_BUILDING_HPP
#define GENERIC_BUILDING_HPP

#define GENERIC_BUILDING_BYTE 0

#include <cassert>

#include "building.hpp"

class GenericBuilding : public Building {
  public:
    GenericBuilding(int hp);
    GenericBuilding(const GenericBuilding &);
    Building *clone() override;

    void update() override;
    void setMap(Map *map) override;
    void takeDamage(int damage) override;
    bool isAlive() override;

    std::string chr() override;
    Color color() override;

    bytedump dump() override;
    GenericBuilding(bytedump dump);

  protected:
    int m_HP;
};

#endif /* GENERIC_BUILDING_HPP */
