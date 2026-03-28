#include "game_state.h"
#include "field.h"
#include <iostream>
#include <algorithm>


// Реализация шаблонного метода
template<typename T>
void GameState::add_entity_impl(const T& entity, std::vector<T>& container, const std::string& error_msg) {
    Position pos = entity.get_position();

    if (!game_field.position_is_valid(pos)) {
        throw std::out_of_range(error_msg);
    }


    if (!game_field.position_is_passable(pos, *this) or pos == Position(0, 0)) {  // УЧТЕНЫ КОЛЛИЗИИ
        Position new_pos = game_field.find_nearest_available_position(pos, *this);
        std::cout << entity.get_name() << " auto-moved from (" << pos.get_x() << "," << pos.get_y()
                  << ") to (" << new_pos.get_x() << "," << new_pos.get_y() << ")\n";

        T corrected_entity = entity;
        corrected_entity.set_position(new_pos);
        container.push_back(corrected_entity);
    } else {
        container.push_back(entity);
    }
}

void GameState::reset() {
    spawners.clear();
    enemies.clear();
    towers.clear();
}


// SPAWNERS
void GameState::add_spawner(const EnemySpawner& spawner) {
    add_entity_impl(spawner, spawners, "Enemy spawner's position is outside field borders");
}

std::vector<EnemySpawner>& GameState::get_spawners() {
    return spawners;
}

const std::vector<EnemySpawner>& GameState::get_spawners() const {
    return spawners;
}


// ENEMIES
void GameState::add_enemy(const Enemy& enemy) {
    add_entity_impl(enemy, enemies, "Enemy's position is outside field borders");
}

std::vector<Enemy>& GameState::get_enemies() {
    return enemies;
}

const std::vector<Enemy>& GameState::get_enemies() const {
    return enemies;
}


// TOWERS
void GameState::add_tower(const EnemyTower& tower) {
    add_entity_impl(tower, towers, "Tower's position is outside field borders");
}

std::vector<EnemyTower>& GameState::get_towers() {
    return towers;
}

const std::vector<EnemyTower>& GameState::get_towers() const {
    return towers;
}


// FIELD
GameField& GameState::get_field() {
    return game_field;
}

const GameField& GameState::get_field() const {
    return game_field;
}

// для обновления уровня
void GameState::set_new_field(const GameField &new_field) const {
    game_field = new_field;
}


// UNIFIED METHODS
void GameState::remove_dead_entities() {
    auto is_dead = [](const Entity& entity) { return !entity.is_alive(); };

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), is_dead), enemies.end());
    spawners.erase(std::remove_if(spawners.begin(), spawners.end(), is_dead), spawners.end());
    towers.erase(std::remove_if(towers.begin(), towers.end(), is_dead), towers.end());
}


Entity* GameState::find_nearest_target(const Position& from, int range) {
    Entity* nearest_target = nullptr;
    int min_distance = range + 1;

    auto process_entity = [&](Entity& entity) {
        if (entity.is_alive()) {
            int distance = from.find_distance_to(entity.get_position());
            if (distance <= range && distance < min_distance) {
                min_distance = distance;
                nearest_target = &entity;
            }
        }
    };

    for (Enemy& enemy : enemies) process_entity(enemy);
    for (EnemySpawner& spawner : spawners) process_entity(spawner);
    for (EnemyTower& tower : towers) process_entity(tower);

    return nearest_target;
}
