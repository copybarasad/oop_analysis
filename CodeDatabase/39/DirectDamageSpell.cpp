#include "DirectDamageSpell.h"
#include "Game.h"
#include "GameField.h"
#include "Cell.h"        

#include <algorithm>
#include <cmath>
#include <stdexcept>

DirectDamageSpell::DirectDamageSpell(int damage, int radius, int cost)
    : damage_(damage),
      radius_(radius),
      cost_(cost) {
    EnsureInvariants();
}

std::string DirectDamageSpell::GetName() const {
    return "Direct damage";
}

int DirectDamageSpell::GetCost() const {
    return cost_;
}

bool DirectDamageSpell::CanCast(const Game &game,
                                std::size_t target_x,
                                std::size_t target_y) const {
    const GameField &field = game.GetField();

    // Цель должна быть в пределах поля
    if (!field.IsInside(target_x, target_y)) {
        return false;
    }

    // Должен быть игрок на поле
    std::size_t player_x = 0;
    std::size_t player_y = 0;
    if (!game.GetPlayerPosition(player_x, player_y)) {
        return false;
    }

    // Проверяем радиус: берём чебышёвское расстояние max(|dx|, |dy|)
    const int dx = static_cast<int>(target_x) - static_cast<int>(player_x);
    const int dy = static_cast<int>(target_y) - static_cast<int>(player_y);
    const int distance = std::max(std::abs(dx), std::abs(dy));
    if (distance > radius_) {
        return false;
    }

    // На цели должен быть враг или вражеское здание
    const Cell::Occupant occupant = field.GetOccupant(target_x, target_y);
    if (occupant != Cell::Occupant::kEnemy &&
        occupant != Cell::Occupant::kEnemyBase) {
        return false;
    }

    return true;
}

bool DirectDamageSpell::Cast(Game &game,
                             std::size_t target_x,
                             std::size_t target_y) {
    // Если нельзя туда кастануть — ход не тратим
    if (!CanCast(game, target_x, target_y)) {
        return false;
    }

    const bool hit = game.DealDirectDamage(target_x, target_y, damage_);
    return hit;
}

void DirectDamageSpell::EnsureInvariants() const {
    if (damage_ <= 0) {
        throw std::logic_error("DirectDamageSpell damage must be > 0");
    }
    if (radius_ <= 0) {
        throw std::logic_error("DirectDamageSpell radius must be > 0");
    }
    if (cost_ < 0) {
        throw std::logic_error("DirectDamageSpell cost must be >= 0");
    }
}
