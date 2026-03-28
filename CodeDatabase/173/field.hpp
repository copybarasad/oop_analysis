#ifndef FIELD_HPP
#define FIELD_HPP

#include <string>

#include "../building/building.hpp"
#include "../combattable.hpp"
#include "../pretty/printable.hpp"
#include "../save/dumpable.hpp"
#include "../trap/trap.hpp"

class Creature;

class Field : public Printable, public Dumpable {
  public:
    static Field *fromByte(std::byte b);

    virtual ~Field() = 0;
    virtual Field *clone() = 0;

    virtual void attack(Combattable *attacker) = 0;
    virtual void damage(int damage) = 0;
    virtual void build(Building *building) = 0;
    virtual void deoccupy() = 0;
    virtual void occupy(Creature *occupier) = 0;
    virtual void placeTrap(Trap *trap) = 0;
    virtual bool canAttack() = 0;
    virtual bool canBuild() = 0;
    virtual bool canOccupy() = 0;
    virtual bool canPlaceTrap() = 0;

    virtual bool isOccupiedByEnemy() = 0;

    virtual void update() = 0;
};

#endif /* FIELD_HPP */
