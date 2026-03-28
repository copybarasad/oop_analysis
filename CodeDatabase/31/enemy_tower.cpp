#include "enemy_tower.hpp"
#include "entities/entity.hpp"
#include <cmath>
#include <format>

EnemyTower::EnemyTower(Field &field, Point position, int health, int damage)
    : Entity(field, position, health), damage(damage) {}

Team EnemyTower::get_team() const {
    return Team::Enemy;
}

EntityType EnemyTower::get_entity_type() const {
    return EntityType::EnemyTower;
}

Entity& EnemyTower::use_spell() {
    for (int x = std::max(0, position.x - radius); x <= std::min(field.get_width() - 1, position.x + radius); x++) {
        for (int y = std::max(0, position.y - radius); y <= std::min(field.get_height() - 1, position.y + radius); y++) {
            if (std::abs(x - position.x) > radius || std::abs(y - position.y) > radius) continue;
            if (Point{x, y} == position) continue;

            Cell &cell = field[{x, y}];
            if (cell.has_entity() && cell.get_entity().get_team() == Team::Player) {
                cell.get_entity().take_damage(damage);
                timer = 2;
                return cell.get_entity();
            }
        }
    }
    return *this;
}

std::string EnemyTower::make_step() {
    if (timer > 0) {
        timer--;
        return "";
    }
    Entity& use_entity = use_spell();
    EntityType use_entity_type = use_entity.get_entity_type();
    if(use_entity_type == EntityType::Player || use_entity_type == EntityType::Ally) {
        return std::format("Enemy tower dealt damage {} to {} at {}\n", damage, use_entity.str(), use_entity.get_position().str()); 
    }
    return "";
}