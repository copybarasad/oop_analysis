#include "enemy.h"
#include <algorithm>

namespace rpg {
    Enemy::Enemy(int health, int damage, const Position &position)
        : health_(std::max(0, health)),
          damage_(std::max(0, damage)),
          position_(position) {
    }

    [[nodiscard]] int Enemy::GetHealth() const {
        return health_;
    }

    [[nodiscard]] int Enemy::GetDamage() const {
        return damage_;
    }

    [[nodiscard]] Position Enemy::GetPosition() const {
        return position_;
    }

    [[nodiscard]] bool Enemy::IsAlive() const {
        return health_ > 0;
    }

    void Enemy::TakeDamage(const int damage) {
        health_ = std::max(0, health_ - damage);
    }

    void Enemy::SetPosition(const Position &pos) {
        position_ = pos;
    }
}
