#include "spell/SpellArea.hpp"
#include "Field.hpp"
#include "Player.hpp"
#include <cstdlib>

// SpellArea.cpp: реализация заклинания удара 2x2 с проверкой радиуса.

namespace {
int manhattan(Pos a, Pos b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
}

bool SpellArea::canCast(const Field&, const Player& p) const {
    return p.mana() >= cost_;
}

bool SpellArea::cast(Field& f, Player& p, std::optional<Pos> target) {
    if (!canCast(f, p)) return false;

    Pos center = target.value_or(p.pos());
    if (manhattan(center, p.pos()) > radius_) {
        return false;
    }

    for (int dy = 0; dy < 2; ++dy) {
        for (int dx = 0; dx < 2; ++dx) {
            const int x = center.x + dx;
            const int y = center.y + dy;
            f.damage_enemy_at(x, y, dmg_);
            f.damage_nest_at(x, y, dmg_);
        }
    }

    p.spend_mana(cost_);
    return true;
}
