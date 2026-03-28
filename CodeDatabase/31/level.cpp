#include "level.hpp"
#include "entities/enemy.hpp"
#include "entities/enemy_building.hpp"
#include "entities/enemy_tower.hpp"
#include <cstdlib>
#include <algorithm>
#include <nlohmann/json.hpp>

Level::Level(Field &field, int level_number) : field(field), enemy_count(0), level_number(level_number) {}

Level::~Level() {
    for (Entity *entity : entities) {
        if (entity->get_entity_type() != EntityType::Player) {
            if (entity->is_entity_alive()) {
                Point pos = entity->get_position();
                if (pos.inside_field(field.get_width(), field.get_height())) {
                    field.get_cell(pos).set_entity(nullptr);
                }
            }
            delete entity;
        }
    }
}

Point Level::rand_pos(int width, int height) {
    Point pos = {rand() % width, rand() % height};
    while(field.get_cell(pos).get_type() != CellType::Default || field.get_cell(pos).has_entity()) {
        pos = {rand() % width, rand() % height};
    }
    return pos;
}

void Level::init_entities(int width, int height) {
    int enemy_count_to_spawn = level_number;
    int building_count = 1 + (level_number - 1) / 2;
    int tower_count = 1 + (level_number - 1) / 3;
    
    int enemy_hp = 12 + (level_number - 1) / 2;
    int enemy_damage = 2 + (level_number - 1) / 5;
    int building_hp = 12 + (level_number - 1);
    int tower_hp = 12 + (level_number - 1);
    int tower_damage = 2 + (level_number - 1) / 3;
    
    for (int i = 0; i < enemy_count_to_spawn; i++) {
        Enemy *enemy = new Enemy(field, rand_pos(width, height), enemy_hp, enemy_damage);
        field.get_cell(enemy->get_position()).set_entity(enemy);
        add_entity(enemy);
    }
    
    for (int i = 0; i < building_count; i++) {
        EnemyBuilding *building = new EnemyBuilding(field, rand_pos(width, height), 25, building_hp, enemy_hp, enemy_damage);
        field.get_cell(building->get_position()).set_entity(building);
        add_entity(building);
    }
    
    for (int i = 0; i < tower_count; i++) {
        EnemyTower *tower = new EnemyTower(field, rand_pos(width, height), tower_hp, tower_damage);
        field.get_cell(tower->get_position()).set_entity(tower);
        add_entity(tower);
    }
}

void Level::add_entity(Entity *entity) {
    entities.push_back(entity);
    if (entity->get_entity_type() == EntityType::Enemy) enemy_count++;
}

void Level::delete_entity(Entity *entity) {
    auto del = std::find(entities.begin(), entities.end(), entity);
    if (del != entities.end()) {
        entities.erase(del);
        if (entity->get_entity_type() == EntityType::Enemy) enemy_count--;
    }
}

std::vector<Entity*>& Level::get_entities() {
    return entities;
}

int Level::get_enemy_count() const {
    return enemy_count;
}

nlohmann::json Level::save() const {
    nlohmann::json j = nlohmann::json::array();
    for (Entity* e : entities) {
        if (e->get_entity_type() == EntityType::Player) continue;
        nlohmann::json entity;
        entity["type"] = (int)e->get_entity_type();
        entity["x"] = e->get_position().x;
        entity["y"] = e->get_position().y;
        entity["health"] = e->get_health();
        
        if (e->get_entity_type() == EntityType::Enemy) {
            Enemy* enemy = (Enemy*)e;
            entity["damage"] = enemy->get_damage();
        } else if (e->get_entity_type() == EntityType::EnemyBuilding) {
            EnemyBuilding* building = (EnemyBuilding*)e;
            entity["enemy_hp"] = building->get_enemy_hp();
            entity["enemy_damage"] = building->get_enemy_damage();
        }
              
        j.push_back(entity);
    }
    return j;
}
