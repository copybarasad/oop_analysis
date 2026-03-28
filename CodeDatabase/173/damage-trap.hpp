#ifndef DAMAGE_TRAP_HPP
#define DAMAGE_TRAP_HPP

#include "trap.hpp"

class DamageTrap : public Trap {
  public:
    DamageTrap(int damage);
    void trigger(Creature *creature) override;

  private:
    int m_Damage;
};

#endif /* DAMAGE_TRAP_HPP */
