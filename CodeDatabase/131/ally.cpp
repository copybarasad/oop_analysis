#include "ally.h"
#include <algorithm>

namespace rpg {
    Ally::Ally(const int health, const int damage, const Position &position)
        : health_(health), damage_(damage), position_(position) {
    }

    [[nodiscard]] Position Ally::GetPosition() const {
        return position_;
    }

    [[nodiscard]] bool Ally::IsAlive() const {
        return health_ > 0;
    }

    [[nodiscard]] int Ally::GetDamage() const {
        return damage_;
    }

    [[nodiscard]] int Ally::GetHealth() const {
        return health_;
    }

    void Ally::TakeDamage(const int damage) {
        health_ = std::max(0, health_ - damage);
    }

    void Ally::SetPosition(const Position &pos) {
        position_ = pos;
    }
}
