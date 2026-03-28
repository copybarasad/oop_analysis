#include "SummonSpell.h"
#include "GameField.h"
#include "Ally.h"
#include "Player.h"
#include <cmath>
#include <random>
#include <vector>
#include <algorithm>

SummonSpell::SummonSpell(int range, int summon_count)
    : Spell("Summon Ally", 0, range, SpellType::SUMMON),
      summon_count_(summon_count),
      ally_health_(40),
      ally_damage_(12) {}

bool SummonSpell::CanCast(int player_x, int player_y, int target_x, int target_y,
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
    return target_cell.IsPassable() && !target_cell.HasPlayer() &&
           !target_cell.HasEnemy() && !target_cell.HasAlly();
}

void SummonSpell::Cast(int player_x, int player_y, int target_x, int target_y,
                        GameField& field, Player& player,
                        std::vector<std::unique_ptr<Enemy>>& enemies,
                        std::vector<std::unique_ptr<EnemyBuilding>>& enemy_buildings,
                        std::vector<std::unique_ptr<EnemyTower>>& enemy_towers,
                        std::vector<std::unique_ptr<Ally>>& allies) {
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < summon_count_; ++i) {
        int ally_x = target_x;
        int ally_y = target_y;

        if (i > 0) {
            std::vector<std::pair<int, int>> adjacent_cells;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    int check_x = target_x + dx;
                    int check_y = target_y + dy;
                    if (field.IsValidPosition(check_x, check_y) &&
                        field.IsPassable(check_x, check_y)) {
                        Cell& cell = field.GetCell(check_x, check_y);
                        if (!cell.HasPlayer() && !cell.HasEnemy() && !cell.HasAlly()) {
                            adjacent_cells.push_back({check_x, check_y});
                        }
                    }
                }
            }

            if (adjacent_cells.empty()) break;

            std::uniform_int_distribution<> dist(0, adjacent_cells.size() - 1);
            auto pos = adjacent_cells[dist(gen)];
            ally_x = pos.first;
            ally_y = pos.second;
        }

        auto ally = std::make_unique<Ally>(ally_x, ally_y, ally_health_, ally_damage_);
        field.GetCell(ally_x, ally_y).SetAlly(true);
        allies.push_back(std::move(ally));
    }
}

void SummonSpell::Enhance() {
    summon_count_ += 1;
    level_++;
}

Spell* SummonSpell::Clone() const {
    return new SummonSpell(*this);
}

int SummonSpell::GetSummonCount() const {
    return summon_count_;
}

int SummonSpell::GetAllyHealth() const {
    return ally_health_;
}

int SummonSpell::GetAllyDamage() const {
    return ally_damage_;
}
