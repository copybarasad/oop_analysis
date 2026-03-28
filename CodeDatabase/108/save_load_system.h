#ifndef OOP_SAVE_LOAD_SYSTEM_H
#define OOP_SAVE_LOAD_SYSTEM_H

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "game_state.h"
#include "player.h"
#include "field.h"
#include "level_system.h"


class SaveLoadSystem {

    std::string save_file = "game_save.txt";

    // Вспомогательные методы для парсинга
    std::vector<std::string> split(const std::string& str, char delimiter);
    CellType parse_cell_type(const std::string& type_str);
    CombatMode parse_combat_mode(const std::string& mode_str);

    // Методы для записи
    void write_player(std::ofstream& file, const Player& player);
    void write_field(std::ofstream& file, const GameField& field);
    void write_entities(std::ofstream& file, const GameState& game_state);

    // Методы для чтения
    void read_player(std::ifstream& file, Player& player);
    void read_field(std::ifstream& file, GameField& field);
    void read_entities(std::ifstream& file, GameState& game_state);

    // дополнительная проверка на корректность файла, что он не поломался
    void validate_save_file_detailed(std::ifstream& file);
    void check_save_exists_and_valid();

public:
    // Сохраняет текущую игру (перезаписывает файл)
    bool save_game(const GameState& game_state,
                   const Player& player,
                   const GameField& field,
                   int current_level,
                   int current_turn);

    // Загружает последнюю сохраненную игру
    bool load_game(GameState& game_state,
                   Player& player,
                   GameField& field,
                   int& current_level,
                   int& current_turn,
                   LevelSystem& level_system);

};





#endif //OOP_SAVE_LOAD_SYSTEM_H

