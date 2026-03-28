#ifndef WALL_FIELD_HPP
#define WALL_FIELD_HPP

#include "../creature.hpp"
#include "field.hpp"
#include "generic-field.hpp"

#define WALL_FIELD_BYTE 3

class WallField : public Field {
  public:
    WallField();
    WallField(const WallField &);
    Field *clone() override;

    void attack(Combattable *attacker) override;
    void damage(int damage) override;
    void build(Building *building) override;
    void deoccupy() override;
    void occupy(Creature *occupier) override;
    void placeTrap(Trap *trap) override;
    bool canAttack() override;
    bool canBuild() override;
    bool canOccupy() override;
    bool canPlaceTrap() override;

    bool isOccupiedByEnemy() override;

    void update() override;

    std::string chr() override;
    Color color() override;

    bytedump dump() override;
};

#endif /* WALL_FIELD_HPP */
