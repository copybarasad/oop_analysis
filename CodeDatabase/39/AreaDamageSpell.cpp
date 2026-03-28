#include "AreaDamageSpell.h"
#include "Game.h"
#include "GameField.h"
#include "Cell.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

AreaDamageSpell::AreaDamageSpell(int damage, int radius, int cost)
    : damage_(damage),
      radius_(radius),
      cost_(cost) {
    EnsureInvariants();
}

std::string AreaDamageSpell::GetName() const {
    return "Area damage 2x2";
}

int AreaDamageSpell::GetCost() const {
    return cost_;
}

bool AreaDamageSpell::CanCast(const Game &game,
                              std::size_t target_x,
                              std::size_t target_y) const {
    const GameField &field = game.GetField();

    const std::size_t max_x = target_x + 1;
    const std::size_t max_y = target_y + 1;
    if (!field.IsInside(target_x, target_y) ||
        !field.IsInside(max_x, max_y)) {
        return false;
    }

    std::size_t player_x = 0;
    std::size_t player_y = 0;
    if (!game.GetPlayerPosition(player_x, player_y)) {
        return false;
    }

    const int center_x = static_cast<int>(target_x) + 1;
    const int center_y = static_cast<int>(target_y) + 1;
    const int dx = center_x - static_cast<int>(player_x);
    const int dy = center_y - static_cast<int>(player_y);
    const int distance = std::max(std::abs(dx), std::abs(dy));
    if (distance > radius_) {
        return false;
    }

    return true;
}

bool AreaDamageSpell::Cast(Game &game,
                           std::size_t target_x,
                           std::size_t target_y) {
    if (!CanCast(game, target_x, target_y)) {
        return false;
    }

    game.DealAreaDamage(target_x, target_y, 2, 2, damage_);
    return true;
}

void AreaDamageSpell::EnsureInvariants() const {
    if (damage_ <= 0) {
        throw std::logic_error("AreaDamageSpell damage must be > 0");
    }
    if (radius_ <= 0) {
        throw std::logic_error("AreaDamageSpell radius must be > 0");
    }
    if (cost_ < 0) {
        throw std::logic_error("AreaDamageSpell cost must be >= 0");
    }
}
