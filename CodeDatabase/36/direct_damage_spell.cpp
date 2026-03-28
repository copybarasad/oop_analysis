#include "direct_damage_spell.h"
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

DirectDamageSpell::DirectDamageSpell(int dmg, int radius) : damage_(dmg), radius_(radius) {}

bool DirectDamageSpell::Cast(Field& field, Player& player, const std::pair<int,int>& target,
                            std::vector<Enemy>& enemies, std::vector<EnemyTower>& towers, std::vector<EnemyBase>& bases) {
    auto [pr, pc] = player.GetPosition();
    auto [tr, tc] = target;
    if (!field.IsInBounds(tr, tc)) {
        std::cout << "Target coordinates (" << tr << "," << tc << ") are out of bounds!\n";
        return false;
    }

    int dist = std::max(std::abs(tr - pr), std::abs(tc - pc));
    int actual_radius = radius_;
    if (player.GetEnhancementStacks() > 0) {
        actual_radius *=(1 << player.GetEnhancementStacks());
        player.ConsumeEnhancementStack();
        std::cout << "Spell empowered! Radius doubled to " << actual_radius << "!\n";
    }
    if (dist > actual_radius) {
        std::cout << "Target is too far! Range: " << actual_radius << " tiles\n";
        return false;
    }

    Cell &cell = field.CellAt(tr, tc);
    if (cell.GetType() == CellType::Enemy) {
        int idx = cell.GetEnemyIndex();
        if (idx >= 0 && idx < static_cast<int>(enemies.size())) {
            enemies[idx].TakeDamage(damage_);
            std::cout << "Direct damage spell strikes enemy at (" << tr << ',' << tc << ") for " << damage_ << " damage!\n";
            if (!enemies[idx].IsAlive()) {
                cell.Clear();
                std::cout << "Enemy killed by spell\n";
                player.AddSpell(CreateRandomSpell());
            }
            return true;
        }
    } else if (cell.GetType() == CellType::Tower) {
        for (size_t i = 0; i < towers.size(); ++i) {
            if (towers[i].Position().first == tr && towers[i].Position().second == tc) {
                towers[i].TakeDamage(damage_);
                std::cout << "Direct damage spell strikes tower at (" << tr << ',' << tc << ") for " << damage_ << " damage!\n";
                if (!towers[i].IsAlive()) {
                    cell.Clear();
                    std::cout << "Tower destroyed by spell\n";
                    player.AddSpell(CreateRandomSpell());
                    towers.erase(towers.begin() + i);
                }
                return true;
            }
        }
    } else if (cell.GetType() == CellType::EnemyBase) {
        for (size_t i = 0; i < bases.size(); ++i) {
            if (bases[i].Position().first == tr && bases[i].Position().second == tc) {
                bases[i].TakeDamage(damage_);
                std::cout << "Direct damage spell strikes base at (" << tr << ',' << tc << ") for " << damage_ << " damage!\n";
                if (!bases[i].IsAlive()) {
                    cell.Clear();
                    std::cout << "Base destroyed by spell\n";
                    player.AddSpell(CreateRandomSpell());
                    bases.erase(bases.begin() + i);
                }
                return true;
            }
        }
    }
    std::cout << "No valid target found at (" << tr << "," << tc << ")\n";
    return false;
}

std::string DirectDamageSpell::Name() const { return "Direct Damage"; }
std::string DirectDamageSpell::Description() const {
    return "Deals " + std::to_string(damage_) + " damage to a single target within " + std::to_string(radius_) + " tiles";
}

int DirectDamageSpell::GetDamage() { return damage_; }
int DirectDamageSpell::GetRadius() { return radius_; }
void DirectDamageSpell::SetDamage(int damage) { damage_ = damage; }
void DirectDamageSpell::SetRadius(int radius) { radius_ = radius; }

