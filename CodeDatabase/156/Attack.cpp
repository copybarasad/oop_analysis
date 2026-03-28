//
// Created by Artem on 24.09.2025.
//

#include "Attack.h"

AttackType Attack::getType() const noexcept {
    return type;
}

int Attack::getDamage() const noexcept {
    return damage;
}
