#include "AreaDamageSpell.h"
#include "GameField.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "Player.h"
#include <cmath>
#include <algorithm>

AreaDamageSpell::AreaDamageSpell(int damage, int range, int area_size)
    : Spell("Area Damage", 0, range, SpellType::AREA_DAMAGE),
      damage_(damage),
      area_size_(area_size) {}

bool AreaDamageSpell::CanCast(int player_x, int player_y, int target_x, int target_y,
                               GameField& field) const {
    int dx = std::abs(target_x - player_x);
    int dy = std::abs(target_y - player_y);
    int distance = std::max(dx, dy);

    if (distance > range_) {
        return false;
    }

    return field.IsValidPosition(target_x, target_y);
}

void AreaDamageSpell::Cast(int player_x, int player_y, int target_x, int target_y,
                            GameField& field, Player& player,
                            std::vector<std::unique_ptr<Enemy>>& enemies,
                            std::vector<std::unique_ptr<EnemyBuilding>>& enemy_buildings,
                            std::vector<std::unique_ptr<EnemyTower>>& enemy_towers,
                            std::vector<std::unique_ptr<Ally>>& allies) {
    for (int dy = 0; dy < area_size_; ++dy) {
        for (int dx = 0; dx < area_size_; ++dx) {
            int check_x = target_x + dx;
            int check_y = target_y + dy;
            
            if (!field.IsValidPosition(check_x, check_y)) {
                continue;
            }

            Cell& cell = field.GetCell(check_x, check_y);

            for (auto& enemy : enemies) {
                if (enemy->GetX() == check_x && enemy->GetY() == check_y) {
                    enemy->TakeDamage(damage_);
                    if (!enemy->IsAlive()) {
                        cell.SetEnemy(false);
                        player.AddScore(enemy->GetScoreValue());
                        player.AddExperience(20);
                    }
                    break;
                }
            }

            for (auto& building : enemy_buildings) {
                if (building->GetX() == check_x && building->GetY() == check_y) {
                    building->TakeDamage(damage_);
                    if (!building->IsAlive()) {
                        cell.SetEnemyBuilding(false);
                        player.AddScore(50);
                    }
                    break;
                }
            }

            for (auto& tower : enemy_towers) {
                if (tower->GetX() == check_x && tower->GetY() == check_y) {
                    tower->TakeDamage(damage_);
                    if (!tower->IsAlive()) {
                        cell.SetEnemyTower(false);
                        player.AddScore(40);
                    }
                    break;
                }
            }
        }
    }
}

void AreaDamageSpell::Enhance() {
    area_size_ += 1;
    level_++;
}

Spell* AreaDamageSpell::Clone() const {
    return new AreaDamageSpell(*this);
}

int AreaDamageSpell::GetDamage() const {
    return damage_;
}

int AreaDamageSpell::GetAreaSize() const {
    return area_size_;
}
