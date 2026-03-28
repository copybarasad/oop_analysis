#include "world.h"
#include "spell.h"
#include "spell_factory.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

World::World(int width, int height)
    : field_(width, height), player_(5, 4, 10), turn_(0), level_(1) {
    player_.SetPosition(0, 0);
    field_.CellAt(0,0).SetType(CellType::Player);

    int br = std::max(1, field_.GetHeight()-2);
    int bc = std::max(1, field_.GetWidth()-2);
    bases_.emplace_back(br, bc, 3, 15);
    field_.CellAt(br, bc).SetType(CellType::EnemyBase);

    for (int i = 0; i < 3; ++i) {
        TryPlaceEnemyRandomly();
    }

    int tr = rand() % field_.GetHeight();
    int tc = rand() % field_.GetWidth();
    if (field_.CellAt(tr, tc).GetType() == CellType::Empty) {
        towers_.emplace_back(tr, tc, 5, 2, 2);
        field_.CellAt(tr, tc).SetType(CellType::Tower);
    }

    player_.AddSpell(CreateRandomSpell());
}

World::World(int width, int height, int level)
    : field_(width, height), player_(5, 4, 10), turn_(0), level_(level) {
    field_.Initialize(level);
    player_.SetPosition(0, 0);
    field_.CellAt(0,0).SetType(CellType::Player);

    int br = std::max(1, field_.GetHeight()-2);
    int bc = std::max(1, field_.GetWidth()-2);
    bases_.emplace_back(br, bc, 3, 15 + 10 * (level - 1));
    field_.CellAt(br, bc).SetType(CellType::EnemyBase);

    int num_enemies = 3 + 2 * (level - 1);
    for (int i = 0; i < num_enemies; ++i) {
        TryPlaceEnemyRandomly();
    }

    int num_towers = 1 + (level - 1);
    for (int i = 0; i < num_towers; ++i) {
        int tr = rand() % field_.GetHeight();
        int tc = rand() % field_.GetWidth();
        if (field_.CellAt(tr, tc).GetType() == CellType::Empty) {
            towers_.emplace_back(tr, tc, 5 + 3 * (level - 1), 2, 2 + (level - 1));
            field_.CellAt(tr, tc).SetType(CellType::Tower);
        }
    }

    player_.AddSpell(CreateRandomSpell());
}

World::World(int width, int height, int level, Player&& player)
    : field_(width, height), player_(std::move(player)), turn_(0), level_(level) {
    field_.Initialize(level);
    bool placed = false;
    for (int r = 0; r < field_.GetHeight() && !placed; ++r) {
        for (int c = 0; c < field_.GetWidth() && !placed; ++c) {
            Cell& cell = field_.CellAt(r, c);
            if (cell.GetType() == CellType::Empty) {
                player_.SetPosition(r, c);
                cell.SetType(CellType::Player);
                placed = true;
            }
        }
    }
    if (!placed) {
        player_.SetPosition(0, 0);
        field_.CellAt(0,0).SetType(CellType::Player);
    }

    int br = std::max(1, field_.GetHeight()-2);
    int bc = std::max(1, field_.GetWidth()-2);
    bases_.emplace_back(br, bc, 3, 15 + 10 * (level - 1));
    field_.CellAt(br, bc).SetType(CellType::EnemyBase);

    int num_enemies = 3 + 2 * (level - 1);
    for (int i = 0; i < num_enemies; ++i) {
        TryPlaceEnemyRandomly();
    }

    int num_towers = 1 + (level - 1);
    for (int i = 0; i < num_towers; ++i) {
        int tr = rand() % field_.GetHeight();
        int tc = rand() % field_.GetWidth();
        if (field_.CellAt(tr, tc).GetType() == CellType::Empty) {
            towers_.emplace_back(tr, tc, 5 + 3 * (level - 1), 2, 2 + (level - 1));
            field_.CellAt(tr, tc).SetType(CellType::Tower);
        }
    }

    player_.AddSpell(CreateRandomSpell());
}

World::World(World&& other) noexcept
    : field_(std::move(other.field_)),
      player_(std::move(other.player_)),
      enemies_(std::move(other.enemies_)),
      bases_(std::move(other.bases_)),
      towers_(std::move(other.towers_)),
      turn_(other.turn_) {
    other.turn_ = 0;
}

World& World::operator=(World&& other) noexcept {
    if (this != &other) {
        field_ = std::move(other.field_);
        player_ = std::move(other.player_);
        enemies_ = std::move(other.enemies_);
        bases_ = std::move(other.bases_);
        towers_ = std::move(other.towers_);
        turn_ = other.turn_;

        other.turn_ = 0;
    }
    return *this;
}

const Field& World::GetField() const { return field_; }
const Player& World::GetPlayer() const { return player_; }
Player& World::MutablePlayer() { return player_; }

bool World::IsGameOver() const { return !player_.IsAlive(); }

bool World::IsBaseAlive() const {
    for (const auto& base : bases_) {
        if (base.IsAlive()) return true;
    }
    return false;
}

int World::GetTurn() const { return turn_; }

void World::SetTurn(int turn) { turn_ = turn; }

int World::GetLevel() const { return level_; }

void World::MovePlayer(int dr, int dc) {
    if (player_.ShouldSkipMove()) {
        std::cout << "Player is slowed and skips this move.\n";
        player_.ConsumedSkipMove();
        return;
    }

    auto [r, c] = player_.GetPosition();
    int nr = r + dr, nc = c + dc;
    if (!field_.IsInBounds(nr, nc)) {
        std::cout << "Move out of bounds\n"; return;
    }
    Cell &target = field_.CellAt(nr, nc);
    if (!target.IsPassable()) {
        std::cout << "Cell not passable\n"; return;
    }
    if (target.ContainsEnemy()) {
        std::cout << "Enemy occupies cell, cannot move\n"; return;
    }

    if (target.GetType() == CellType::Trap) {
        int trap_dmg = target.GetTrapDamage();
        player_.TakeDamage(trap_dmg);
        std::cout << "Player stepped on trap for " << trap_dmg << " damage\n";
        target.SetType(CellType::Empty);
        target.SetTrapDamage(0);
        if (!player_.IsAlive()) {
            std::cout << "Player died on trap\n";
            return;
        }
    }

    field_.CellAt(r,c).SetType(CellType::Empty);
    bool wasSlow = (target.GetType() == CellType::Slow);
    target.SetType(CellType::Player);
    player_.SetPosition(nr, nc);
    if (wasSlow) {
        player_.SetSkipNextMove(true);
        std::cout << "Stepped on slow tile, will skip next move\n";
    }
}

void World::PlayerAttack() {
    auto [pr, pc] = player_.GetPosition();
    int range = player_.IsRanged() ? 2 : 1;
    int dmg = player_.CurrentDamage();
    int hit = 0;
    for (int dr = -range; dr <= range; ++dr) {
        for (int dc = -range; dc <= range; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int r = pr + dr, c = pc + dc;
            if (!field_.IsInBounds(r,c)) continue;
            Cell &cell = field_.CellAt(r,c);
            if (cell.ContainsEnemy()) {
                int idx = cell.GetEnemyIndex();
                if (idx >= 0 && idx < static_cast<int>(enemies_.size())) {
                    enemies_[idx].TakeDamage(dmg);
                    std::cout << "Hit enemy at " << r << "," << c << " for " << dmg << "\n";
                    ++hit;
                    if (!enemies_[idx].IsAlive()) {
                        cell.Clear();
                        std::cout << "Enemy defeated\n";
                        player_.AddSpell(CreateRandomSpell());
                    }
                }
            } else if (cell.GetType() == CellType::Tower) {
                for (size_t i = 0; i < towers_.size(); ++i) {
                    if (towers_[i].Position().first == r && towers_[i].Position().second == c) {
                        towers_[i].TakeDamage(dmg);
                        std::cout << "Hit tower at " << r << "," << c << " for " << dmg << "\n";
                        ++hit;
                        if (!towers_[i].IsAlive()) {
                            cell.Clear();
                            std::cout << "Tower destroyed\n";
                            player_.AddSpell(CreateRandomSpell());
                            towers_.erase(towers_.begin() + i);
                        }
                        break;
                    }
                }
            } else if (cell.GetType() == CellType::EnemyBase) {
                for (size_t i = 0; i < bases_.size(); ++i) {
                    if (bases_[i].Position().first == r && bases_[i].Position().second == c) {
                        bases_[i].TakeDamage(dmg);
                        std::cout << "Hit base at " << r << "," << c << " for " << dmg << "\n";
                        ++hit;
                        if (!bases_[i].IsAlive()) {
                            cell.Clear();
                            std::cout << "Base destroyed\n";
                            bases_.erase(bases_.begin() + i);
                        }
                        break;
                    }
                }
            }
        }
    }
    if (hit == 0) std::cout << "No targets in range\n";
}

bool World::PlayerCastSpell(int idx, const std::pair<int,int>& target) {
    if (idx < 0 || idx >= player_.GetSpellCount()) {
        std::cout << "Invalid spell index\n"; return false;
    }

    const Spell* spell_ptr = player_.GetSpell(idx);
    if (!spell_ptr) { std::cout << "Spell missing\n"; return false; }

    Spell* nonconst = const_cast<Spell*>(spell_ptr);
    bool result = nonconst->Cast(field_, player_, target, enemies_, towers_, bases_);

    if (result) {
        player_.RemoveSpellAt(idx);
    }
    return result;
}

void World::AdvanceTurn() {
    ++turn_;
    UpdateAllies();
    ApplyPendingSpellDamages();
    TowerAttackPhase();
    UpdateEnemies();
    EnemySpawnPhase();
}

void World::TowerAttackPhase() {
    auto player_pos = player_.GetPosition();
    for (auto &tower : towers_) {
        if (tower.ShouldCast(player_pos)) {
            player_.TakeDamage(tower.DirectDamageSpell());
            std::cout << "Tower at " << tower.Position().first << ',' << tower.Position().second
                      << " attacked player for " << tower.DirectDamageSpell() << " damage! Player HP: " << player_.Health() << "\n";
        }
    }
}

void World::PrintStatus() const {
    std::cout << "HP: " << player_.Health() << "/" << player_.MaxHealth() << " | ";
    std::cout << "Mode: " << (player_.IsRanged() ? "Ranged" : "Melee") << " | ";
    std::cout << "Damage: " << player_.CurrentDamage() << " | ";
    std::cout << "Spells: " << player_.GetSpellCount() << " | ";
    std::cout << "Enhancement: " << player_.GetEnhancementStacks() << " | ";
    std::cout << "Turn: " << turn_ << "\n";
    
    if (player_.ShouldSkipMove()) {
        std::cout << "Status: Slowed (will skip next move)\n";
    }
}

void World::SwitchPlayerMode() {
    player_.SwitchCombatMode();
    std::cout << "Switching mode to " << (player_.IsRanged() ? "Ranged" : "Melee") << "\n";
}

void World::TryPlaceEnemyRandomly() {
    for (int attempt = 0; attempt < 20; ++attempt) {
        int r = rand() % field_.GetHeight();
        int c = rand() % field_.GetWidth();
        Cell &cell = field_.CellAt(r,c);
        if (cell.GetType() == CellType::Empty) {
            enemies_.push_back(Enemy(5 + 2 * (level_ - 1), 1 + (level_ - 1)));
            int idx = static_cast<int>(enemies_.size() - 1);
            cell.SetType(CellType::Enemy);
            cell.SetEnemyIndex(idx);
            return;
        }
    }
}

void World::ApplyPendingSpellDamages() {
    for (int r = 0; r < field_.GetHeight(); ++r) {
        for (int c = 0; c < field_.GetWidth(); ++c) {
            Cell &cell = field_.CellAt(r,c);
            int dmg = cell.GetTrapDamage();
            if (dmg <= 0) continue;
            if (cell.GetType() == CellType::Trap) continue;
            if (cell.ContainsEnemy()) {
                int idx = cell.GetEnemyIndex();
                if (idx >= 0 && idx < static_cast<int>(enemies_.size())) {
                    enemies_[idx].TakeDamage(dmg);
                    std::cout << "Spell dealt " << dmg << " damage to enemy at " << r << "," << c << "\n";
                    if (!enemies_[idx].IsAlive()) {
                        cell.Clear();
                        std::cout << "Enemy killed by spell\n";
                        player_.AddSpell(CreateRandomSpell());
                    }
                }
            }
            cell.SetTrapDamage(0);
        }
    }
}

void World::UpdateEnemies() {
    struct Move { int src_r, src_c, dst_r, dst_c, enemy_idx; };
    std::vector<Move> planned;

    for (int r = 0; r < field_.GetHeight(); ++r) {
        for (int c = 0; c < field_.GetWidth(); ++c) {
            Cell &cell = field_.CellAt(r, c);
            if (!cell.ContainsEnemy()) continue;
            int enemy_idx = cell.GetEnemyIndex();
            if (enemy_idx < 0 || enemy_idx >= static_cast<int>(enemies_.size())) continue;
            Enemy &e = enemies_[enemy_idx];
            if (!e.IsAlive()) {
                cell.Clear();
                continue;
            }
            auto [pr, pc] = player_.GetPosition();
            int dr = (pr > r) ? 1 : ((pr < r) ? -1 : 0);
            int dc = (pc > c) ? 1 : ((pc < c) ? -1 : 0);
            int nr = r + dr;
            int nc = c + dc;
            if (!field_.IsInBounds(nr, nc)) continue;
            Cell &target = field_.CellAt(nr, nc);
            if (target.GetType() == CellType::Player) {
                player_.TakeDamage(e.Damage());
                std::cout << "Enemy at " << r << ',' << c << " attacked player for " << e.Damage()
                          << " | Player HP: " << player_.Health() << "\n";
                continue;
            }
            if (target.GetType() == CellType::Trap) {
                int trap_dmg = target.GetTrapDamage();
                e.TakeDamage(trap_dmg);
                std::cout << "Enemy at " << r << ',' << c << " stepped on trap for " << trap_dmg << " damage\n";

                field_.CellAt(r, c).Clear();

                target.SetType(CellType::Empty);
                target.SetTrapDamage(0);

                if (!e.IsAlive()) {
                    std::cout << "Enemy died on trap\n";
                    player_.AddSpell(CreateRandomSpell());
                } else {
                    target.SetType(CellType::Enemy);
                    target.SetEnemyIndex(enemy_idx);
                }
            } else if (target.IsPassable() && !target.ContainsEnemy() && target.GetType() != CellType::EnemyBase &&
                       target.GetType() != CellType::Wall && target.GetType() != CellType::Ally &&
                       target.GetType() != CellType::Tower) {
                planned.push_back({r, c, nr, nc, enemy_idx});
            }
        }
    }

    for (auto &m : planned) {
        Cell &src = field_.CellAt(m.src_r, m.src_c);
        Cell &dst = field_.CellAt(m.dst_r, m.dst_c);
        if (!dst.IsPassable() || dst.ContainsEnemy() || dst.GetType() == CellType::Player ||
            dst.GetType() == CellType::Tower) continue;
        dst.SetType(CellType::Enemy);
        dst.SetEnemyIndex(m.enemy_idx);
        src.Clear();
    }
}

void World::UpdateAllies() {
    //nothing happens
    return;
}

void World::EnemySpawnPhase() {
    for (auto &b : bases_) {
        if (!b.ShouldSpawn()) continue;
        auto [br, bc] = b.Position();
        const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        for (auto &d : dirs) {
            int nr = br + d[0], nc = bc + d[1];
            if (!field_.IsInBounds(nr, nc)) continue;
            Cell &cell = field_.CellAt(nr, nc);
            if (cell.GetType() == CellType::Empty) {
                enemies_.push_back(Enemy(5 + 2 * (level_ - 1), 1 + (level_ - 1)));
                int idx = static_cast<int>(enemies_.size() - 1);
                cell.SetType(CellType::Enemy);
                cell.SetEnemyIndex(idx);
                std::cout << "Enemy spawned at " << nr << ',' << nc << "\n";
                break;
            }
        }
    }
}

void World::ClearEnemies() {
    enemies_.clear();
}

void World::ClearBases() {
    bases_.clear();
}

void World::ClearTowers() {
    towers_.clear();
}

void World::SetEnemies(std::vector<Enemy> enemies) {
    enemies_ = std::move(enemies);
}

void World::SetBases(std::vector<EnemyBase> bases) {
    bases_ = std::move(bases);
}

void World::SetTowers(std::vector<EnemyTower> towers) {
    towers_ = std::move(towers);
}
