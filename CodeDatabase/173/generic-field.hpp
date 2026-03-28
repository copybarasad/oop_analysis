#ifndef GENERIC_FIELD_HPP
#define GENERIC_FIELD_HPP

#include "field.hpp"

#define GENERIC_FIELD_BYTE 1

class GenericField : public Field {
  public:
    GenericField();
    GenericField(const GenericField &);
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

  protected:
    Creature *m_CreatureSlot;
    Building *m_BuildingSlot;
    Trap *m_TrapSlot;
};

#endif /* GENERIC_FIELD_HPP */
