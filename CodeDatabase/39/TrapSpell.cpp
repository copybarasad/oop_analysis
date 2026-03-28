#include "TrapSpell.h"

#include "Game.h"
#include "GameField.h"
#include "Cell.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iostream>

TrapSpell::TrapSpell(int damage, int radius, int cost)
    : damage_(damage),
      radius_(radius),
      cost_(cost) {
    EnsureInvariants();
}

std::string TrapSpell::GetName() const {
    return "Trap";
}

int TrapSpell::GetCost() const {
    return cost_;
}

bool TrapSpell::CanCast(const Game &game,
                        std::size_t target_x,
                        std::size_t target_y) const {
    const GameField &field = game.GetField();

    if (!field.IsInside(target_x, target_y)) {
        return false;
    }

    // не ставим ловушку в стену
    if (field.GetTerrain(target_x, target_y) == Cell::Terrain::kBlocked) {
        return false;
    }

    // радиус относительно игрока
    std::size_t player_x = 0;
    std::size_t player_y = 0;
    if (!game.GetPlayerPosition(player_x, player_y)) {
        return false;
    }

    int dx = static_cast<int>(target_x) - static_cast<int>(player_x);
    int dy = static_cast<int>(target_y) - static_cast<int>(player_y);
    int dist = std::max(std::abs(dx), std::abs(dy)); // квадрат вокруг игрока

    if (dist > radius_) {
        return false;
    }

    if (field.GetOccupant(target_x, target_y) != Cell::Occupant::kNone) {
        return false;
    }

    if (field.HasTrap(target_x, target_y)) {
        return false;
    }

    return true;
}

bool TrapSpell::Cast(Game &game,
                     std::size_t target_x,
                     std::size_t target_y) {
    if (!CanCast(game, target_x, target_y)) {
        return false;
    }

    GameField &field = game.GetField();
    field.PlaceTrap(target_x, target_y, damage_);
    std::cout << "Trap placed at (" << target_x << "," << target_y << ").\n";
    return true;
}

void TrapSpell::EnsureInvariants() const {
    if (damage_ <= 0) {
        throw std::logic_error("TrapSpell damage must be > 0");
    }
    if (radius_ <= 0) {
        throw std::logic_error("TrapSpell radius must be > 0");
    }
    if (cost_ < 0) {
        throw std::logic_error("TrapSpell cost must be >= 0");
    }
}
