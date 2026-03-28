#include "enemy_building.hpp"
#include "field/cell.hpp"
#include "entity.hpp"
#include "field/field.hpp"
#include <cstdlib>
#include <vector>
#include "enemy.hpp"
#include "game/game.hpp"
#include <format>

EnemyBuilding::EnemyBuilding(Field &field, Point position, int period, int health, int enemy_hp, int enemy_damage)
    : Entity(field, position, health), period(period), enemy_hp(enemy_hp), enemy_damage(enemy_damage), pending_enemy(nullptr) {}

Point EnemyBuilding::create_enemy() {
    if (pending_enemy != nullptr) return {-1, -1};
    
    int radius = 1;
    std::vector<Point> candidates;
    for (int x = std::max(0, position.x - radius); x <= std::min(field.get_width() - 1, position.x + radius); x++) {
        for (int y = std::max(0, position.y - radius); y <= std::min(field.get_height() - 1, position.y + radius); y++) {
            Cell &cell = field[{x, y}];
            if (cell.get_type() != CellType::Impassible && !cell.has_entity()) {
                candidates.push_back({x, y});
            }
        }
    }
    if (!candidates.empty()) {
        Point point = candidates[rand() % candidates.size()];
        pending_enemy = new Enemy(field, point, enemy_hp, enemy_damage);
        return point;
    }
}

std::string EnemyBuilding::make_step() {
    if (timer == 0) {
        timer = period;
        Point point = create_enemy();
        if(point.x != -1 and point.y != -1)
        return std::format("Enemy building made enemy at enemy at {} with hp {}\n", point.str(), enemy_hp);
    }
    else {
        timer--;
    }
    return "";
}

Entity* EnemyBuilding::get_pending_entity() {
    Entity* result = pending_enemy;
    pending_enemy = nullptr;
    return result;
}

Team EnemyBuilding::get_team() const {
    return Team::Enemy;
}

EntityType EnemyBuilding::get_entity_type() const {
    return EntityType::EnemyBuilding;
}

int EnemyBuilding::get_enemy_hp() const {
    return enemy_hp;
}

int EnemyBuilding::get_enemy_damage() const {
    return enemy_damage;
}