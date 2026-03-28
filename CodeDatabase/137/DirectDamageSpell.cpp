#include "DirectDamageSpell.h"
#include "GameField.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "Player.h"
#include <cmath>
#include <algorithm>

DirectDamageSpell::DirectDamageSpell(int damage, int range)
    : Spell("Direct Damage", 0, range, SpellType::DIRECT_DAMAGE),
      damage_(damage) {}

bool DirectDamageSpell::CanCast(int player_x, int player_y, int target_x, int target_y,
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
    return target_cell.HasEnemy() || target_cell.HasEnemyBuilding() || target_cell.HasEnemyTower();
}

void DirectDamageSpell::Cast(int player_x, int player_y, int target_x, int target_y,
                              GameField& field, Player& player,
                              std::vector<std::unique_ptr<Enemy>>& enemies,
                              std::vector<std::unique_ptr<EnemyBuilding>>& enemy_buildings,
                              std::vector<std::unique_ptr<EnemyTower>>& enemy_towers,
                              std::vector<std::unique_ptr<Ally>>& allies) {
    Cell& target_cell = field.GetCell(target_x, target_y);

    for (auto& enemy : enemies) {
        if (enemy->GetX() == target_x && enemy->GetY() == target_y) {
            enemy->TakeDamage(damage_);
            if (!enemy->IsAlive()) {
                target_cell.SetEnemy(false);
                player.AddScore(enemy->GetScoreValue());
                player.AddExperience(20);
            }
            return;
        }
    }

    for (auto& building : enemy_buildings) {
        if (building->GetX() == target_x && building->GetY() == target_y) {
            building->TakeDamage(damage_);
            if (!building->IsAlive()) {
                target_cell.SetEnemyBuilding(false);
                player.AddScore(50);
            }
            return;
        }
    }

    for (auto& tower : enemy_towers) {
        if (tower->GetX() == target_x && tower->GetY() == target_y) {
            tower->TakeDamage(damage_);
            if (!tower->IsAlive()) {
                target_cell.SetEnemyTower(false);
                player.AddScore(40);
            }
            return;
        }
    }
}

void DirectDamageSpell::Enhance() {
    range_ += 1;
    level_++;
}

Spell* DirectDamageSpell::Clone() const {
    return new DirectDamageSpell(*this);
}

int DirectDamageSpell::GetDamage() const {
    return damage_;
}
