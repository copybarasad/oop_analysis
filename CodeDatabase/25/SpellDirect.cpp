#include "spell/SpellDirect.hpp"
#include "Field.hpp"
#include "Player.hpp"
#include <cstdlib>

// SpellDirect.cpp: direct spell implementation (single target in range).

namespace {
int manhattan(Pos a, Pos b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
}

bool SpellDirect::canCast(const Field&, const Player& p) const {
    return p.mana() >= cost_;
}

bool SpellDirect::cast(Field& f, Player& p, std::optional<Pos> target) {
    if (!canCast(f, p)) return false;

    const Pos center = p.pos();
    std::optional<Pos> chosen = target;
    if (chosen && manhattan(*chosen, center) > radius_) {
        return false;
    }

    if (!chosen) {
        chosen = f.find_target_in_radius(center, radius_);
    }
    if (!chosen) return false;

    const Pos t = *chosen;
    bool hit = f.damage_enemy_at(t.x, t.y, dmg_);
    if (!hit) {
        hit = f.damage_nest_at(t.x, t.y, dmg_);
    }
    if (!hit) return false;

    p.spend_mana(cost_);
    return true;
}
