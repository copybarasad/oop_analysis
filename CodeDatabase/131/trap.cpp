#include "trap.h"

namespace rpg {
    Trap::Trap(const Position& position, int damage)
        : position_(position), damage_(damage) {}
    
    [[nodiscard]] Position Trap::GetPosition() const {
        return position_;
    }
    
    [[nodiscard]] int Trap::GetDamage() const {
        return damage_;
    }
}


