#include "Weapon.h"
#include "Entity.h"
#include "Object.h"
#include <cmath>

int Weapon::hit(Object *target) const {
    Entity *entityPtr = dynamic_cast<Entity *>(target);
    if (entityPtr) {
        const Coordinates pos_target = entityPtr->getPosition();
        const Coordinates pos_owner = this->owner_->getPosition();
        const int distance = ceil(sqrt(
            pow(static_cast<long long>(pos_owner.x_) - static_cast<long long>(pos_target.x_), 2)
            + pow(static_cast<long long>(pos_owner.y_) - static_cast<long long>(pos_target.y_), 2)
        ));
        if (distance <= this->range_) {
            entityPtr->onHit(this->damage_);
            return 0;
        }
    }
    return 1;
}
