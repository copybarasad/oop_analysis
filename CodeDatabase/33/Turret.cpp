#include "Turret.h"
#include <cstdlib>

Turret::Turret(Coord pos, int hp, int damage, int range, int cooldown_turns)
    : Character(hp, damage, pos),
      range_(range),
      cooldown_max_(cooldown_turns),
      cooldown_left_(0) {}

int Turret::Range() const {
    return range_;
}

int Turret::Cooldown() const { return cooldown_max_; }

int Turret::CooldownTimer() const { return cooldown_left_; }

void Turret::SetCooldownTimer(int v) { cooldown_left_ = v; }

int Turret::ManhattanDistance(const Coord& a, const Coord& b) const {
    int dx = a.X() - b.X();
    if (dx < 0) dx = -dx;
    int dy = a.Y() - b.Y();
    if (dy < 0) dy = -dy;
    return dx + dy;
}

bool Turret::TryFireAtPlayer(const Coord& player_pos, Player& player) {
    if (cooldown_left_ != 0) return false;

    if (ManhattanDistance(Pos(), player_pos) <= range_) {
        player.TakeDamage(Damage());
        cooldown_left_ = cooldown_max_;
        return true;
    }
    return false;
}

void Turret::CooldownTick() {
    if (cooldown_left_ > 0) {
        --cooldown_left_;
    }
}
