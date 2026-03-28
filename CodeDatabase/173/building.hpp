#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <stdexcept>

#include "../positionable.hpp"
#include "../pretty/printable.hpp"
#include "../save/dumpable.hpp"

class Map;

class Building : public Printable, public Positionable, public Dumpable {
  public:
    virtual ~Building() = 0;
    virtual Building *clone() = 0;
    static Building *fromByteDump(bytedump bytes);

    virtual void setMap(Map *map) = 0;
    virtual void update() = 0;
    virtual void takeDamage(int damage) = 0;
    virtual bool isAlive() = 0;
};

#endif /* BUILDING_HPP */
