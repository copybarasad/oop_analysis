#include "Ally.h"

Ally::Ally(int hp, int dmg) : Enemy(hp, dmg) {
    name_ = "Ally";
}

std::shared_ptr<Entity> Ally::clone() const { return std::make_shared<Ally>(*this); }
