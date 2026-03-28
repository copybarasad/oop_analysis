#include "Trap.h"
#include "Entity.h"

Trap::Trap(int dmg) : Entity(1, dmg, "Trap") {}

std::shared_ptr<Entity> Trap::clone() const { return std::make_shared<Trap>(*this); }
