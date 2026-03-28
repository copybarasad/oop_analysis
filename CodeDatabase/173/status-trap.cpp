#include "status-trap.hpp"
#include "../creature.hpp"

StatusTrap::StatusTrap(Status status) { m_Status = status; }
void StatusTrap::trigger(Creature *creature) { creature->addStatus(m_Status); }
