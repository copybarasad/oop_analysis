#ifndef STATUS_TRAP_HPP
#define STATUS_TRAP_HPP

#include "../status.hpp"
#include "trap.hpp"

class StatusTrap : public Trap {
  public:
    StatusTrap(Status status);
    void trigger(Creature *creature) override;

  private:
    Status m_Status;
};

#endif /* STATUS_TRAP_HPP */
