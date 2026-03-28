#include "save_load_system.h"
#include <iostream>
#include <sstream>


// HELP METHODS
std::vector<std::string> SaveLoadSystem::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(str);

    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

CellType SaveLoadSystem::parse_cell_type(const std::string& type_str) {
    if (type_str == "EMPTY") return CellType::EMPTY;
    if (type_str == "OBSTACLE") return CellType::OBSTACLE;
    if (type_str == "SLOW") return CellType::SLOW;
    if (type_str == "TRAP") return CellType::TRAP;
    throw std::invalid_argument("Unknown cell type: " + type_str);
}

CombatMode SaveLoadSystem::parse_combat_mode(const std::string& mode_str) {
    if (mode_str == "MELEE") return CombatMode::MELEE;
    if (mode_str == "RANGED") return CombatMode::RANGED;
    throw std::invalid_argument("Unknown combat mode: " + mode_str);
}


// дополнительная полная проверка (что не учли ранее):
// количество параметров, наличие всех обязательных блоков
void SaveLoadSystem::validate_save_file_detailed(std::ifstream& file) {
    std::string line;
    std::vector<std::string> required_sections = {"LEVEL", "TURN", "PLAYER", "FIELD", "SPELLS"};
    std::vector<bool> found_sections(required_sections.size(), false);
    int line_number = 0;

    while (std::getline(file, line)) {
        line_number++;

        if (line.empty()) continue;

        std::vector<std::string> tokens = split(line, ':');
        if (tokens.empty()) continue;

        const std::string& type = tokens[0];

        // Проверяем обязательные секции
        for (size_t i = 0; i < required_sections.size(); ++i) {
            if (type == required_sections[i]) {
                found_sections[i] = true;
            }
        }

        // Детальная валидация форматов
        if (type == "LEVEL") {
            if (tokens.size() != 2) {
                throw std::runtime_error("LINE " + std::to_string(line_number) +
                                       ": LEVEL requires exactly 2 parameters");
            }
            try {
                int value = std::stoi(tokens[1]);
                if (value < 0) throw std::runtime_error("Level must be non-negative");
            } catch (const std::exception&) {
                throw std::runtime_error("LINE " + std::to_string(line_number) +
                                       ": Invalid level value: " + tokens[1]);
            }
        }
        else if (type == "TURN") {
            if (tokens.size() != 2) {
                throw std::runtime_error("LINE " + std::to_string(line_number) +
                                       ": TURN requires exactly 2 parameters");
            }
            try {
                int value = std::stoi(tokens[1]);
                if (value < 0) throw std::runtime_error("Turn must be non-negative");
            } catch (const std::exception&) {
                throw std::runtime_error("LINE " + std::to_string(line_number) +
                                       ": Invalid turn value: " + tokens[1]);
            }
        }
        else if (type == "PLAYER") {
            if (tokens.size() != 10) {
                throw std::runtime_error("LINE " + std::to_string(line_number) +
                                       ": PLAYER requires exactly 9 parameters");
            }
        }
        else if (type == "FIELD") {
            if (tokens.size() != 3) {
                throw std::runtime_error("LINE " + std::to_string(line_number) +
                                       ": FIELD requires exactly 3 parameters");
            }
        }
    }

    // Проверяем отсутствующие секции
    for (size_t i = 0; i < required_sections.size(); ++i) {
        if (!found_sections[i]) {
            throw std::runtime_error("Missing required section: " + required_sections[i]);
        }
    }
}


void SaveLoadSystem::check_save_exists_and_valid() {
    std::ifstream file(save_file);
    if (!file.is_open()) {
        throw std::runtime_error("Saving file " + save_file + " wasn't found ");
    }

    try {
        validate_save_file_detailed(file);
        file.close();
    } catch (const std::exception& e) {
        file.close();
        throw std::runtime_error(std::string("Saving error: ") + e.what());
    }
}



// Методы записи
void SaveLoadSystem::write_player(std::ofstream& file, const Player& player) {
    if (!file.is_open()) throw std::runtime_error("File is not open for writing");

    file << "PLAYER:" << player.get_name() << ":"
         << player.get_health() << ":"
         << player.get_position().get_x() << ":"
         << player.get_position().get_y() << ":"
         << player.get_score() << ":"
         << player.get_melee_damage() << ":"
         << player.get_ranged_damage() << ":"
         << (player.get_combat_mode() == CombatMode::MELEE ? "MELEE" : "RANGED") << ":"
         << player.get_is_slowed_info() << '\n';

    // Заклинания
    file << "SPELLS:";
    for (int i = 0; i < player.get_spell_hand().get_spell_count(); ++i) {
        if (i > 0) file << ":";
        file << player.get_spell_hand().get_spell_name(i);
    }
    file << "\n";
}

void SaveLoadSystem::write_field(std::ofstream& file, const GameField& field) {
    if (!file.is_open()) throw std::runtime_error("File is not open for writing");

    file << "FIELD:" << field.get_width() << ":" << field.get_height() << "\n";

    for (int y = 0; y < field.get_height(); ++y) {
        for (int x = 0; x < field.get_width(); ++x) {
            Cell cell = field.get_cell_at(Position(x, y));
            file << "CELL:" << x << ":" << y << ":";
            switch(cell.get_type()) {
                case CellType::EMPTY: file << "EMPTY"; break;
                case CellType::OBSTACLE: file << "OBSTACLE"; break;
                case CellType::SLOW: file << "SLOW"; break;
                case CellType::TRAP: file << "TRAP:" << cell.get_trap_damage(); break;
            }
            file << "\n";
        }
    }
}

void SaveLoadSystem::write_entities(std::ofstream& file, const GameState& game_state) {
    if (!file.is_open()) throw std::runtime_error("File is not open for writing");

    for (const auto& enemy : game_state.get_enemies()) {
        if (enemy.is_alive()) {
            file << "ENEMY:" << enemy.get_name() << ":"
                 << enemy.get_health() << ":"
                 << enemy.get_position().get_x() << ":"
                 << enemy.get_position().get_y() << ":"
                 << enemy.get_damage() << "\n";
        }
    }

    for (const auto& spawner : game_state.get_spawners()) {
        if (spawner.is_alive()) {
            file << "SPAWNER:" << spawner.get_name() << ":"
                 << spawner.get_health() << ":"
                 << spawner.get_position().get_x() << ":"
                 << spawner.get_position().get_y() << ":"
                 << spawner.get_spawn_cooldown() << ":"
                 << spawner.get_max_cooldown() << "\n";
        }
    }

    for (const auto& tower : game_state.get_towers()) {
        if (tower.is_alive()) {
            file << "TOWER:" << tower.get_name() << ":"
                 << tower.get_health() << ":"
                 << tower.get_position().get_x() << ":"
                 << tower.get_position().get_y() << ":"
                 << tower.get_damage() << ":"
                 << tower.get_attack_range() << ":"
                 << tower.get_attack_cooldown() << ":"
                 << tower.get_max_cooldown() << "\n";
        }
    }
}


// Основной метод сохранения
bool SaveLoadSystem::save_game(const GameState& game_state,
                               const Player& player,
                               const GameField& field,
                               int current_level,
                               int current_turn) {
    try {
        std::ofstream file(save_file);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open the file " + save_file);
        }

        // Основная информация
        file << "LEVEL:" << current_level << "\n";
        file << "TURN:" << current_turn << "\n";

        write_player(file, player);

        write_field(file, field);

        write_entities(file, game_state);

        file.close();
        std::cout << "Игра успешно сохранена!\n";
        return true;

    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Saving error: ") + e.what());
    }
}




// Методы чтения
void SaveLoadSystem::read_player(std::ifstream& file, Player& player) {
    std::string line;
    std::getline(file, line); // PLAYER строка
    std::vector<std::string> tokens = split(line, ':');


    // Восстанавливаем основные параметры игрока
    std::string name = tokens[1];
    int health = std::stoi(tokens[2]);
    int pos_x = std::stoi(tokens[3]);
    int pos_y = std::stoi(tokens[4]);
    int score = std::stoi(tokens[5]);
    int melee_dmg = std::stoi(tokens[6]);
    int ranged_dmg = std::stoi(tokens[7]);
    CombatMode mode = parse_combat_mode(tokens[8]);
    bool is_slowed = std::stoi(tokens[9]);


    // Устанавливаем параметры
    player.set_health(health);
    player.set_position(Position(pos_x, pos_y));
    player.set_score(score);
    player.set_melee_damage(melee_dmg);
    player.set_ranged_damage(ranged_dmg);
    player.set_combat_mode(mode);
    if (is_slowed) player.apply_slow_effect();


    player.get_spell_hand().clear_spells();  // сброс заклинаний из предыдущего состояния

    // Читаем заклинания
    std::getline(file, line); // SPELLS строка
    tokens = split(line, ':');
    if (tokens.size() >= 2) {  // иначе нет заклинаний => не записываем
        std::vector<std::string> spell_names = split(tokens[1], ':');
        player.get_spell_hand().load_spells_from_names(spell_names);
    }

}

void SaveLoadSystem::read_field(std::ifstream& file, GameField& field) {
    std::string line;
    std::getline(file, line); // FIELD строка
    std::vector<std::string> tokens = split(line, ':');

    int width = std::stoi(tokens[1]);
    int height = std::stoi(tokens[2]);

    // Создаем поле нужного размера
    field = GameField(width, height);

    // Читаем клетки через for по всем + проверка последовательности и наличия всех клеток + их структуры
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (!std::getline(file, line)) {
                throw std::runtime_error("Unexpected end of file while reading field cells");
            }

            if (line.empty() || line.find("CELL:") != 0) {
                throw std::runtime_error("Expected CELL line, got: " + line);
            }

            tokens = split(line, ':');
            if (tokens.size() < 4) {
                throw std::runtime_error("Invalid CELL format: " + line);
            }


            // Проверяем координаты
            int cell_x = std::stoi(tokens[1]);
            int cell_y = std::stoi(tokens[2]);

            if (cell_x != x || cell_y != y) {
                throw std::runtime_error(
                    "Cell coordinate mismatch. Expected: " + std::to_string(x) + "," + std::to_string(y) +
                    ", Got: " + std::to_string(cell_x) + "," + std::to_string(cell_y)
                );
            }

            CellType cell_type = parse_cell_type(tokens[3]);

            if (cell_type == CellType::TRAP) {
                int trap_damage = std::stoi(tokens[4]);
                if (tokens.size() != 5) {
                    throw std::runtime_error("TRAP Cell arguments amount error in " + line);
                }
                field.set_or_remove_trap(Position(x, y), trap_damage);
            } else {
                if (tokens.size() != 4) {
                    throw std::runtime_error("Cell arguments amount error in " + line);
                }
                field.set_cell_type(Position(x, y), cell_type);
            }
        }
    }

}


void SaveLoadSystem::read_entities(std::ifstream& file, GameState& game_state) {
    std::string line;

    while (std::getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        std::vector<std::string> tokens = split(line, ':');
        if (tokens.empty()) {
            throw std::runtime_error("Invalid entity line format: " + line);
        }

        const std::string& type = tokens[0];

        // Проверяем известные типы сущностей
        if (type != "ENEMY" && type != "SPAWNER" && type != "TOWER") {
            throw std::runtime_error("Unknown entity type: " + type);
        }

        try {
            if (type == "ENEMY") {

                const std::string& name = tokens[1];
                int health = std::stoi(tokens[2]);
                int x = std::stoi(tokens[3]);
                int y = std::stoi(tokens[4]);
                int damage = std::stoi(tokens[5]);

                // Валидация значений
                if (health <= 0) throw std::runtime_error("Enemy health must be positive");
                if (damage <= 0) throw std::runtime_error("Enemy damage must be positive");

                Enemy enemy(name, Position(x, y), health, damage, 1);
                game_state.add_enemy(enemy);
            }
            else if (type == "SPAWNER") {

                const std::string& name = tokens[1];
                int health = std::stoi(tokens[2]);
                int x = std::stoi(tokens[3]);
                int y = std::stoi(tokens[4]);
                int cooldown = std::stoi(tokens[5]);
                int max_cooldown = std::stoi(tokens[6]);

                // Валидация значений
                if (health <= 0) throw std::runtime_error("Spawner health must be positive");
                if (cooldown < 0 || cooldown > max_cooldown) {
                    throw std::runtime_error("Invalid spawner cooldown: " + std::to_string(cooldown));
                }

                EnemySpawner spawner(name, Position(x, y), health, max_cooldown);
                for (int i = 0; i < (max_cooldown - cooldown); i++) {
                    spawner.decrease_cooldown();
                }
                game_state.add_spawner(spawner);
            }
            else if (type == "TOWER") {


                const std::string& name = tokens[1];
                int health = std::stoi(tokens[2]);
                int x = std::stoi(tokens[3]);
                int y = std::stoi(tokens[4]);
                int damage = std::stoi(tokens[5]);
                int range = std::stoi(tokens[6]);
                int cooldown = std::stoi(tokens[7]);
                int max_cooldown = std::stoi(tokens[8]);

                // Валидация значений
                if (health <= 0) throw std::runtime_error("Tower health must be positive");
                if (damage <= 0) throw std::runtime_error("Tower damage must be positive");
                if (range <= 0) throw std::runtime_error("Tower range must be positive");
                if (cooldown < 0 || cooldown > max_cooldown) {
                    throw std::runtime_error("Invalid tower cooldown: " + std::to_string(cooldown));
                }

                EnemyTower tower(name, Position(x, y), health, range, damage, max_cooldown);
                for (int i = 0; i < (max_cooldown - cooldown); i++) {
                    tower.decrease_cooldown();
                }
                game_state.add_tower(tower);
            }

        } catch (const std::exception& e) {
            throw std::runtime_error(std::string("Failed to load entity: ") + e.what());
        }
    }
}


// основной метод загрузки
bool SaveLoadSystem::load_game(GameState& game_state,
                               Player& player,
                               GameField& field,
                               int& current_level,
                               int& current_turn,
                               LevelSystem& level_system) {
    try {

        check_save_exists_and_valid();  // проверка файла перед загрузкой информации оттуда на соответствие шаблону + некоторые проверки остались в самих read_*

        std::ifstream file(save_file);
        if (!file.is_open()) {
            throw std::runtime_error("failed to open save_file.txt");
        }

        // Очищаем текущее состояние
        game_state.reset();   // убираем сущностей, а поле обновляется через копирование в методе read_field

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::vector<std::string> tokens = split(line, ':');
            if (tokens.empty()) continue;

            const std::string& type = tokens[0];

            if (type == "LEVEL") {
                current_level = std::stoi(tokens[1]);
                level_system.set_current_level(current_level);   // нужно для синхронизации, иначе сбивался счетчик lvl
            }
            else if (type == "TURN") {
                current_turn = std::stoi(tokens[1]);
            }
            else if (type == "PLAYER") {
                // Возвращаемся в начало строки и читаем игрока полностью
                file.seekg(-static_cast<int>(line.length() + 1), std::ios_base::cur);
                read_player(file, player);
            }
            else if (type == "FIELD") {
                // Возвращаемся назад и читаем поле полностью
                file.seekg(-static_cast<int>(line.length() + 1), std::ios_base::cur);
                read_field(file, field);
            }
            else if (type == "ENEMY" || type == "SPAWNER" || type == "TOWER") {
                // Возвращаемся назад и читаем все сущности
                file.seekg(-static_cast<int>(line.length() + 1), std::ios_base::cur);
                read_entities(file, game_state);
            }
        }

        file.close();
        std::cout << "Игра успешно загружена!\n";
        return true;

    } catch (const std::exception& e) { // тут cout
        throw std::runtime_error(std::string("Loading error: ") + e.what());
    }
}
