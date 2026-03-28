#include "TrapSpell.h"
#include "GameField.h"
#include "Player.h"
#include <cmath>
#include <algorithm>

TrapSpell::TrapSpell(int damage, int range)
    : Spell("Trap", 0, range, SpellType::TRAP),
      damage_(damage) {}

bool TrapSpell::CanCast(int player_x, int player_y, int target_x, int target_y,
                         GameField& field) const {
    int dx = std::abs(target_x - player_x);
    int dy = std::abs(target_y - player_y);
    int distance = std::max(dx, dy);

    if (distance > range_) {
        return false;
    }

    if (!field.IsValidPosition(target_x, target_y)) {
        return false;
    }

    const Cell& target_cell = field.GetCell(target_x, target_y);
    return target_cell.IsPassable() && !target_cell.HasTrap() && !target_cell.HasPlayer();
}

void TrapSpell::Cast(int player_x, int player_y, int target_x, int target_y,
                      GameField& field, Player& player,
                      std::vector<std::unique_ptr<Enemy>>& enemies,
                      std::vector<std::unique_ptr<EnemyBuilding>>& enemy_buildings,
                      std::vector<std::unique_ptr<EnemyTower>>& enemy_towers,
                      std::vector<std::unique_ptr<Ally>>& allies) {
    Cell& target_cell = field.GetCell(target_x, target_y);
    target_cell.SetTrap(true, damage_);
}

void TrapSpell::Enhance() {
    damage_ += 10;
    level_++;
}

Spell* TrapSpell::Clone() const {
    return new TrapSpell(*this);
}

int TrapSpell::GetDamage() const {
    return damage_;
}
