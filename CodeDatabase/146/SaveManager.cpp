#include "SaveManager.h"

void SaveManager::save_game(GameState& state, const std::string& filename)
{
    try {
        FileHandler file(filename, std::ios::out | std::ios::trunc);
        std::fstream& fs = file.get_stream();

        // Записываем уровень и размер поля
        fs << state.current_level << "\n";
        fs << state.area_width << " " << state.area_height << "\n";

        // Данные игрока
        fs << state.player_health << " " << state.player_damage << " "
            << state.player_experience << "\n";
        fs << state.player_coords.x << " " << state.player_coords.y << "\n";
        fs << state.player_hand_size << "\n";

        // Враги
        fs << state.enemies.size() << "\n";
        for (const auto& enemy : state.enemies) {
            fs << enemy.health << " " << enemy.damage << " "
                << enemy.coords.x << " " << enemy.coords.y << "\n";
        }

        // Союзники 
        fs << state.summons.size() << "\n";
        for (const auto& summon : state.summons) {
            fs << summon.health << " " << summon.damage << " "
                << summon.coords.x << " " << summon.coords.y << "\n";
        }

        // Здания
        fs << state.buildings.size() << "\n";
        for (const auto& building : state.buildings) {
            fs << building.type << " " << building.health << " "
                << building.coords.x << " " << building.coords.y << " "
                << building.spawn_step << "\n";
        }

        fs << state.cell_types.size() << "\n";
        for (const auto& row : state.cell_types) {
            for (int type : row) {
                fs << type << " ";
            }
            fs << "\n";
        }

        if (fs.fail()) {
            throw FileWriteException(filename);
        }

        std::cout << "Game saved successfully to " << filename << "\n";

    }
    catch (const FileOpenException& e) {
        std::cerr << "Error: " << e.what() << "\n";
        throw;
    }
    catch (const FileWriteException& e) {
        std::cerr << "Error: " << e.what() << "\n";
        throw;
    }
}

GameState SaveManager::load_game(const std::string& filename)
{
    GameState state;

    try {
        FileHandler file(filename, std::ios::in);
        std::fstream& fs = file.get_stream();

        // Читаем уровень и размер поля
        if (!(fs >> state.current_level)) {
            throw CorruptedDataException("Invalid level data");
        }
        if (!(fs >> state.area_width >> state.area_height)) {
            throw CorruptedDataException("Invalid area dimensions");
        }

        // Данные игрока
        if (!(fs >> state.player_health >> state.player_damage >> state.player_experience)) {
            throw CorruptedDataException("Invalid player stats");
        }
        if (!(fs >> state.player_coords.x >> state.player_coords.y)) {
            throw CorruptedDataException("Invalid player coordinates");
        }
        if (!(fs >> state.player_hand_size)) {
            throw CorruptedDataException("Invalid hand size");
        }

        // Враги
        size_t enemy_count;
        if (!(fs >> enemy_count)) {
            throw CorruptedDataException("Invalid enemy count");
        }

        for (size_t i = 0; i < enemy_count; ++i) {
            GameState::EnemyData enemy;
            if (!(fs >> enemy.health >> enemy.damage >> enemy.coords.x >> enemy.coords.y)) {
                throw CorruptedDataException("Invalid enemy data");
            }
            state.enemies.push_back(enemy);
        }

        // Союзники
        size_t summon_count;
        if (!(fs >> summon_count)) {
            throw CorruptedDataException("Invalid summon count");
        }
        for (size_t i = 0; i < summon_count; ++i) {
            GameState::SummonData summon;
            if (!(fs >> summon.health >> summon.damage
                >> summon.coords.x >> summon.coords.y)) {
                throw CorruptedDataException("Invalid summon data");
            }
            state.summons.push_back(summon);
        }

        // Здания
        size_t building_count;
        if (!(fs >> building_count)) {
            throw CorruptedDataException("Invalid building count");
        }

        for (size_t i = 0; i < building_count; ++i) {
            GameState::BuildingData building;
            if (!(fs >> building.type >> building.health >> building.coords.x
                >> building.coords.y >> building.spawn_step)) {
                throw CorruptedDataException("Invalid building data");
            }
            state.buildings.push_back(building);
        }

        if (fs.fail() && !fs.eof()) {
            throw FileReadException(filename);
        }

        size_t rows;
        if (!(fs >> rows)) {
            throw CorruptedDataException("Invalid cell types data");
        }

        state.cell_types.resize(rows);
        for (size_t y = 0; y < rows; ++y) {
            state.cell_types[y].resize(state.area_width);
            for (int x = 0; x < state.area_width; ++x) {
                int type;
                if (!(fs >> type)) {
                    throw CorruptedDataException("Invalid cell type");
                }
                state.cell_types[y][x] = type;
            }
        }

        std::cout << "Game loaded successfully from " << filename << "\n";
        return state;

    }
    catch (const FileOpenException& e) {
        std::cerr << "Error: " << e.what() << "\n";
        throw;
    }
    catch (const CorruptedDataException& e) {
        std::cerr << "Error: " << e.what() << "\n";
        throw;
    }
    catch (const FileReadException& e) {
        std::cerr << "Error: " << e.what() << "\n";
        throw;
    }
}