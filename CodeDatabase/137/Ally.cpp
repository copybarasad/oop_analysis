#include "Ally.h"
#include <algorithm>

Ally::Ally(int x, int y, int health, int damage)
    : health_(health),
      max_health_(health),
      damage_(damage),
      x_(x),
      y_(y) {}

int Ally::GetHealth() const {
    return health_;
}

int Ally::GetMaxHealth() const {
    return max_health_;
}

int Ally::GetDamage() const {
    return damage_;
}

int Ally::GetX() const {
    return x_;
}

int Ally::GetY() const {
    return y_;
}

void Ally::TakeDamage(int damage) {
    health_ = std::max(0, health_ - damage);
}

void Ally::SetPosition(int x, int y) {
    x_ = x;
    y_ = y;
}

bool Ally::IsAlive() const {
    return health_ > 0;
}
