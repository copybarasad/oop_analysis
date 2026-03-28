#include "actor.hpp"

Actor::Actor(int hp, int damage, Point pos)
    : hp_(hp), base_damage_(damage), pos_(pos) {
    if (hp_ <= 0) throw std::invalid_argument("Actor: hp must be > 0");
    if (base_damage_ < 0) throw std::invalid_argument("Actor: damage must be >= 0");
}

Actor::~Actor() = default;

int Actor::HP() const noexcept { return hp_; }
int Actor::BaseDamage() const noexcept { return base_damage_; }
Point Actor::Pos() const noexcept { return pos_; }

void Actor::SetPos(Point p) noexcept { pos_ = p; }

bool Actor::IsAlive() const noexcept { return hp_ > 0; }

void Actor::TakeDamage(int dmg) {
    if (dmg < 0) throw std::invalid_argument("TakeDamage: dmg < 0");
    hp_ -= dmg;
    if (hp_ < 0) hp_ = 0;
}