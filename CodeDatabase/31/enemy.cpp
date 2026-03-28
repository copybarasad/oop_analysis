#include "enemy.hpp"
#include "entities/entity.hpp"
#include "field/field.hpp"
#include <format>

Enemy::Enemy(Field &field, Point position, int health, int damage)
    : AIEntity(field, position, health), damage_cnt(damage) {}

int Enemy::get_damage() const {
    return damage_cnt;
}

Entity& Enemy::enemy_step(Direction direction) {
    Point target_pos = position + get_direction_offset(direction);
    
    bool motion = move_to(get_direction_offset(direction));
    
    if (!motion && target_pos.inside_field(field.get_width(), field.get_height())) {
        Cell &cell = field.get_cell(target_pos);
        if (cell.has_entity() && cell.get_entity().get_team() == Team::Player) {
            Entity& entity = cell.get_entity();
            entity.take_damage(damage_cnt);
            return entity;
        }
    }
    
    return *this;
}

std::string Enemy::make_step() {
    Entity* target = find_closest_target(Team::Player);
    
    if (target != nullptr) {
        Direction dir = get_best_dir(target->get_position());
        if (dir != Direction::TotalCount) {
            Point from = position;
            Entity& entity = enemy_step(dir);
            if (entity.get_entity_type() == EntityType::Enemy) {
                Point to = position;
                return std::format("Enemy moved from {} to {}\n", from.str(), to.str());
            }    
            else {
                return std::format("Enemy attacked {} at {}, and dealt damage {}\n", entity.str(), entity.get_position().str(), damage_cnt);
            }   
        }
    }

    int random_dir = rand() % (int)Direction::TotalCount;
    Point from = position;
        Entity& entity = enemy_step((Direction)random_dir);
        if (entity.get_entity_type() == EntityType::Enemy) {
            Point to = position;
            return std::format("Enemy moved from {} to {}\n", from.str(), to.str());
        }
        else {
            return std::format("Enemy attacked {} at {} and dealt damage {}\n", entity.str(), entity.get_position().str(), damage_cnt);
        }    
    return "";
}

Team Enemy::get_team() const {
    return Team::Enemy;
}

EntityType Enemy::get_entity_type() const {
    return EntityType::Enemy;
}