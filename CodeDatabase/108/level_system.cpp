#include "level_system.h"
#include <random>

#include "player.h"

LevelSystem::LevelSystem() : current_level_index(0) {
    initialize_levels();
}

void LevelSystem::initialize_levels() {
    // Уровень 1 - обучение
    levels.push_back({
        1,                          // level_number
        15, 10,                     // field_width, height
        2,                          // enemy_count
        1,                          // spawner_count
        1,                          // tower_count
        30,                         // enemy_health
        5,                          // enemy_damage
        50,                         // spawner_health
        70,                         // tower_health
        7,                          // tower_damage
        4                           // tower_range
    });

    // Уровень 2 - средняя сложность
    levels.push_back({
        2,                          // level_number
        18, 12,                     // field_width, height
        3,                          // enemy_count
        2,                          // spawner_count
        1,                          // tower_count
        40,                         // enemy_health
        8,                          // enemy_damage
        70,                         // spawner_health
        80,                         // tower_health
        10,                         // tower_damage
        5                           // tower_range
    });

    // Уровень 3 - сложный
    levels.push_back({
        3,                          // level_number
        22, 15,                     // field_width, height
        4,                          // enemy_count
        3,                          // spawner_count
        2,                          // tower_count
        50,                         // enemy_health
        10,                         // enemy_damage
        80,                         // spawner_health
        90,                         // tower_health
        12,                         // tower_damage
        5                           // tower_range
    });
}


Position LevelSystem::generate_random_position(const GameField& field) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_x(0, field.get_width() - 1);
    std::uniform_int_distribution<> dist_y(0, field.get_height() - 1);

    Position pos(dist_x(gen), dist_y(gen));
    return pos;
}


// MAIN LOGIC
void LevelSystem::load_level(int level_number, GameState& game_state, Player& player) {
    if (level_number < 1 || level_number > levels.size()) {
        throw std::runtime_error("Invalid level number: " + std::to_string(level_number));
    }

    current_level_index = level_number - 1;
    const LevelConfig& config = levels[current_level_index];

    // Очищаем игровой мир
    game_state.reset();

    // Создаем новое поле
    GameField new_field(config.field_width, config.field_height);
    game_state.set_new_field(new_field);


    // Устанавливаем игрока в стартовую позицию, восстанавливаем здоровье до макс
    player.set_position(Position(0, 0));
    player.set_health(player.get_max_health());

    // Спавним врагов, согласно уровню
    for (int i = 0; i < config.enemy_count; ++i) {
        Position pos = generate_random_position(new_field);
        Enemy enemy("Enemy_L" + std::to_string(config.level_number) + "_" + std::to_string(i+1),
                   pos, config.enemy_health, config.enemy_damage, 1);    // если плохая клетка, то оно автоматически сдвинется на ближайшую свободную
        game_state.add_enemy(enemy);
    }

    // Спавним спавнеры
    for (int i = 0; i < config.spawner_count; ++i) {
        Position pos = generate_random_position(new_field);
        EnemySpawner spawner("Spawner_L" + std::to_string(config.level_number) + "_" + std::to_string(i+1),
                            pos, config.spawner_health, 8); // cooldown 8 ходов
        game_state.add_spawner(spawner);
    }

    // Спавним башни
    for (int i = 0; i < config.tower_count; ++i) {
        Position pos = generate_random_position(new_field);
        EnemyTower tower("Tower_L" + std::to_string(config.level_number) + "_" + std::to_string(i+1),
                        pos, config.tower_health, config.tower_range,
                        config.tower_damage, 6); // cooldown 6 ходов
        game_state.add_tower(tower);
    }
}

bool LevelSystem::is_level_completed(const GameState& game_state) const {
    // Уровень завершен, когда все вражеские сущности уничтожены
    // так как падшие отчищаются, то условие прохождения все вектора пустые
    return game_state.get_enemies().empty() && game_state.get_spawners().empty() && game_state.get_towers().empty();
}

int LevelSystem::get_current_level() const {
    return current_level_index + 1;
}

bool LevelSystem::next_level(GameState& game_state, Player& player) {
    if (!has_next_level()) return false;

    current_level_index++;
    load_level(current_level_index + 1, game_state, player);
    return true;
}

bool LevelSystem::has_next_level() const {
    return current_level_index < levels.size() - 1;
}

// для синхронизации
void LevelSystem::set_current_level(int level) {
    if (level < 1 || level > levels.size()) {
        throw std::runtime_error("Invalid level number");
    }
    current_level_index = level - 1;
}

