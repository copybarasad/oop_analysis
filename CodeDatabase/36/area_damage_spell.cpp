#include "area_damage_spell.h"
#include "field.h"
#include "player.h"
#include "enemy.h"
#include "enemy_tower.h"
#include "enemy_base.h"
#include "cell.h"
#include "cell_type.h"
#include "spell_factory.h"
#include <iostream>
#include <algorithm>
#include <vector>

AreaDamageSpell::AreaDamageSpell(int dmg, int area) : damage_(dmg), area_size_(area) {}

bool AreaDamageSpell::Cast(Field& field, Player& player, const std::pair<int,int>& target,
                          std::vector<Enemy>& enemies, std::vector<EnemyTower>& towers, std::vector<EnemyBase>& bases) {
    auto [pr, pc] = player.GetPosition();
    auto [tr, tc] = target;
    if (!field.IsInBounds(tr, tc)) {
        std::cout << "Target coordinates (" << tr << "," << tc << ") are out of bounds!\n";
        return false;
    }

    int cast_range = 2;
    int dist = std::max(std::abs(tr - pr), std::abs(tc - pc));
    if (dist > cast_range) {
        std::cout << "Target is too far! Casting range: " << cast_range << " tiles\n";
        return false;
    }

    int actual_area = area_size_;
    if (player.GetEnhancementStacks() > 0) {
        actual_area *= (1 << player.GetEnhancementStacks());
        player.ConsumeEnhancementStack();
        std::cout << "Spell empowered! Area increased to " << actual_area << "!\n";
    }

    int half = actual_area / 2;
    int targets_hit = 0;
    std::cout << "Casting area damage spell at (" << tr << ',' << tc << ")!\n";

    for (int dr = -half; dr <= half; ++dr) {
        for (int dc = -half; dc <= half; ++dc) {
            int r = tr + dr;
            int c = tc + dc;
            if (!field.IsInBounds(r,c)) continue;
            Cell& cell = field.CellAt(r, c);
            if (cell.GetType() == CellType::Enemy) {
                int idx = cell.GetEnemyIndex();
                if (idx >= 0 && idx < static_cast<int>(enemies.size())) {
                    enemies[idx].TakeDamage(damage_);
                    std::cout << "Hit enemy at (" << r << "," << c << ")\n";
                    targets_hit++;
                    if (!enemies[idx].IsAlive()) {
                        cell.Clear();
                        std::cout << "Enemy killed by spell\n";
                        player.AddSpell(CreateRandomSpell());
                    }
                }
            } else if (cell.GetType() == CellType::Tower) {
                bool tower_hit = false;
                for (size_t i = 0; i < towers.size(); ++i) {
                    if (towers[i].Position().first == r && towers[i].Position().second == c) {
                        towers[i].TakeDamage(damage_);
                        std::cout << "Hit tower at (" << r << "," << c << ")\n";
                        targets_hit++;
                        if (!towers[i].IsAlive()) {
                            cell.Clear();
                            std::cout << "Tower destroyed by spell\n";
                            player.AddSpell(CreateRandomSpell());
                            towers.erase(towers.begin() + i);
                        }
                        tower_hit = true;
                        break;
                    }
                }
                if (!tower_hit) {
                    cell.SetType(CellType::Empty);
                }
            } else if (cell.GetType() == CellType::EnemyBase) {
                for (size_t i = 0; i < bases.size(); ++i) {
                    if (bases[i].Position().first == r && bases[i].Position().second == c) {
                        bases[i].TakeDamage(damage_);
                        std::cout << "Hit base at (" << r << "," << c << ")\n";
                        targets_hit++;
                        if (!bases[i].IsAlive()) {
                            cell.Clear();
                            std::cout << "Base destroyed by spell\n";
                            player.AddSpell(CreateRandomSpell());
                            bases.erase(bases.begin() + i);
                        }
                        break;
                    }
                }
            }
        }
    }

    std::cout << "Area spell affected " << targets_hit << " targets in " << area_size_ << "x" << area_size_ << " area!\n";
    return true;
}


int AreaDamageSpell::GetDamage() {
    return damage_;
}

int AreaDamageSpell::GetArea() {
    return area_size_;
}

void AreaDamageSpell::SetDamage(int damage) {
    damage_ = damage;
}

void AreaDamageSpell::SetArea(int area_size) {
    area_size_ = area_size;
}

std::string AreaDamageSpell::Name() const { return "Area Damage"; }
std::string AreaDamageSpell::Description() const {
    return "Deals " + std::to_string(damage_) + " damage in " + std::to_string(area_size_) + "x" + std::to_string(area_size_) + " area";
}