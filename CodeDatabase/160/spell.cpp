#include "../include/items/spell.h"
#include "../include/core/game_board.h"
#include <cmath>

Spell::Spell(SpellType type, const std::string& name, const std::string& description, int cost)
    : type_(type), name_(name), description_(description), cost_(cost) {}

HealSpell::HealSpell(int heal_amount, int cost)
    : Spell(SpellType::Heal, "Исцеление", "Восстанавливает здоровье", cost), heal_amount_(heal_amount) {}

void HealSpell::Use(GameBoard& board) {
    board.HealPlayer(heal_amount_);
}

Spell* HealSpell::Clone() const {
    return new HealSpell(heal_amount_, cost_);
}

DamageSpell::DamageSpell(int damage, int cost)
    : Spell(SpellType::Damage, "Удар", "Наносит урон врагам в радиусе", cost), damage_(damage) {}

void DamageSpell::Use(GameBoard& board) { }

void DamageSpell::UseWithTarget(GameBoard& board, int target_index) {
    if (target_index < 0 || target_index >= static_cast<int>(board.enemies().size())) {
        return;
    }
    
    if (!board.enemies()[target_index].is_alive()) {
        return;
    }
    
    Position player_pos = board.PlayerPosition();
    Position enemy_pos = board.EnemyPosition(target_index);
    
    int distance = std::abs(player_pos.row() - enemy_pos.row());
    if (distance > 1) distance = 1;
    
    int col_dist = std::abs(player_pos.col() - enemy_pos.col());
    if (col_dist > 1) col_dist = 1;
    
    if (distance <= 1 && col_dist <= 1 && !(distance == 0 && col_dist == 0)) {
        int total_damage = damage_ + board.player().enhancement().damage_boost() + board.player().spell_power_bonus();
        board.DamageEnemy(target_index, total_damage);
    }
}

std::vector<int> DamageSpell::GetTargetsInRange(const GameBoard& board) const {
    std::vector<int> targets;
    Position player_pos = board.PlayerPosition();
    
    for (size_t i = 0; i < board.enemies().size(); ++i) {
        if (!board.enemies()[i].is_alive()) continue;
        
        Position enemy_pos = board.EnemyPosition(i);
        
        int row_dist = std::abs(player_pos.row() - enemy_pos.row());
        int col_dist = std::abs(player_pos.col() - enemy_pos.col());
        
        if (row_dist <= 1 && col_dist <= 1 && !(row_dist == 0 && col_dist == 0)) {
            targets.push_back(i);
        }
    }
    
    return targets;
}

Spell* DamageSpell::Clone() const {
    return new DamageSpell(damage_, cost_);
}

SplashSpell::SplashSpell(int damage, int width, int height, int cost)
    : Spell(SpellType::Splash, "Удар по площади", "Наносит урон на площади перед игроком", cost), 
      damage_(damage), width_(width), height_(height) {}

void SplashSpell::Use(GameBoard& board) {
}

void SplashSpell::UseInDirection(GameBoard& board, Direction dir) {
    std::vector<Position> area = GetAreaPositions(board, dir);

    int total_damage = damage_ + board.player().enhancement().damage_boost() + board.player().spell_power_bonus();

    for (const Position& pos : area) {
        for (size_t i = 0; i < board.enemies().size(); ++i) {
            if (!board.enemies()[i].is_alive()) continue;
            
            Position enemy_pos = board.EnemyPosition(i);
            if (enemy_pos.row() == pos.row() && enemy_pos.col() == pos.col()) {
                board.DamageEnemy(i, total_damage);
            }
        }

        for (size_t i = 0; i < board.enemy_towers().size(); ++i) {
            if (!board.enemy_towers()[i].is_alive()) continue;
            
            Position tower_pos = board.TowerPosition(i);
            if (tower_pos.row() == pos.row() && tower_pos.col() == pos.col()) {
                board.DamageTower(i, total_damage);
            }
        }
    }
}

std::vector<Position> SplashSpell::GetAreaPositions(const GameBoard& board, Direction dir) const {
    std::vector<Position> positions;
    Position player_pos = board.PlayerPosition();
    
    int start_row = player_pos.row();
    int start_col = player_pos.col();

    int enhanced_width = width_ + board.player().enhancement().area_boost();
    int enhanced_height = height_ + board.player().enhancement().area_boost();

    switch (dir) {
        case Direction::Up:
            for (int h = 1; h <= enhanced_height; ++h) {
                for (int w = -(enhanced_width / 2); w <= (enhanced_width / 2); ++w) {
                    Position pos(start_row - h, start_col + w);
                    if (board.InBoard(pos)) {
                        positions.push_back(pos);
                    }
                }
            }
            break;
            
        case Direction::Down:
            for (int h = 1; h <= enhanced_height; ++h) {
                for (int w = -(enhanced_width / 2); w <= (enhanced_width / 2); ++w) {
                    Position pos(start_row + h, start_col + w);
                    if (board.InBoard(pos)) {
                        positions.push_back(pos);
                    }
                }
            }
            break;
            
        case Direction::Left:
            for (int w = 1; w <= enhanced_height; ++w) {
                for (int h = -(enhanced_width / 2); h <= (enhanced_width / 2); ++h) {
                    Position pos(start_row + h, start_col - w);
                    if (board.InBoard(pos)) {
                        positions.push_back(pos);
                    }
                }
            }
            break;
            
        case Direction::Right:
            for (int w = 1; w <= enhanced_height; ++w) {
                for (int h = -(enhanced_width / 2); h <= (enhanced_width / 2); ++h) {
                    Position pos(start_row + h, start_col + w);
                    if (board.InBoard(pos)) {
                        positions.push_back(pos);
                    }
                }
            }
            break;
    }
    
    return positions;
}

Spell* SplashSpell::Clone() const {
    return new SplashSpell(damage_, width_, height_, cost_);
}

TrapSpell::TrapSpell(int damage, int cost)
    : Spell(SpellType::Trap, "Ловушка", "Устанавливает ловушку под игроком", cost), 
      damage_(damage) {}

void TrapSpell::Use(GameBoard& board) {
    board.SetPlayerHasTrap(true);
}

Spell* TrapSpell::Clone() const {
    return new TrapSpell(damage_, cost_);
}

SummonSpell::SummonSpell(int summon_count, int cost)
    : Spell(SpellType::Summon, "Призыв союзника", "Призывает союзника рядом с игроком", cost), 
      summon_count_(summon_count) {}

void SummonSpell::Use(GameBoard& board) {
    int enhanced_count = summon_count_ + board.player().enhancement().summon_boost();
    board.SummonFriends(enhanced_count);
}

Spell* SummonSpell::Clone() const {
    return new SummonSpell(summon_count_, cost_);
}

EnhanceSpell::EnhanceSpell(int cost)
    : Spell(SpellType::Enhance, "Усиление", "Усиливает следующее заклинание", cost) {}

void EnhanceSpell::Use(GameBoard& board) {
    board.player().enhancement().add_boost();
}

Spell* EnhanceSpell::Clone() const {
    return new EnhanceSpell(cost_);
}