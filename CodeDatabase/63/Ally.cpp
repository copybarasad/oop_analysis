#include "Ally.h"

Ally::Ally(unsigned int h, unsigned int d, unsigned int viewDiameter) : Entity(h, d, EntityTeam::ALLY, 'A', viewDiameter, 0) {}

Ally::Ally(json &data) : Entity(data, EntityTeam::ALLY) {}

std::unique_ptr<Entity> Ally::clone() const {
    return std::make_unique<Ally>(*this);
}
