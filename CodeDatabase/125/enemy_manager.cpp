#include "enemy_manager.h"
#include "game_field.h" 
#include "player.h"       
#include <random>
#include <algorithm>

void EnemyManager::AddEnemy(const Enemy& e) {
	enemies_.push_back(e);
}

void EnemyManager::InitializeEnemies(const GameField& field, int count, int base_hp, int base_damage) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> x_dist(0, field.GetWidth() - 1);
    std::uniform_int_distribution<> y_dist(0, field.GetHeight() - 1);

    for (int i = 0; i < count; ++i) {
        Position pos;
        do {
            pos = {x_dist(gen), y_dist(gen)};
        } while (!field.IsWalkable(pos));

        int hp = base_hp + i * 5;
        int damage = base_damage;

        enemies_.emplace_back(pos, hp, damage);
    }
}

std::vector<Enemy>& EnemyManager::GetEnemies() { return enemies_; }
const std::vector<Enemy>& EnemyManager::GetEnemies() const { return enemies_; }
std::vector<EnemyTower>& EnemyManager::GetTowers() { return towers_; }
const std::vector<EnemyTower>& EnemyManager::GetTowers() const { return towers_; }

void EnemyManager::MoveEnemies(Player& player, GameField& field) {
	for (auto& e : enemies_)
		if (e.IsAlive())
			e.MoveTowards(player, field, enemies_);
}

bool EnemyManager::AnyAlive() const {
	for (const auto& e : enemies_)
		if (e.IsAlive()) return true;
	return false;
}

void EnemyManager::AddTower(const Position& pos, GameField& field) {
    if (field.IsValidPosition(pos)) {
        field.SetCell(pos, CellType::Obstacle);
        towers_.emplace_back(pos);
    }
}

void EnemyManager::UpdateTowers(Player& player, GameField& field) {
    for (auto& tower : towers_) {
        tower.Update(player, field);
    }
}

void EnemyManager::Clear() {
    enemies_.clear();
    towers_.clear();
}
