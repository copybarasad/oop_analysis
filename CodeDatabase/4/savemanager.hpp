#pragma once

#include <string>
#include <vector>
#include <filesystem>

class GameEngine;

class SaveManager {
public:
    static const std::string SAVE_DIR;
    static void save_to_file(const GameEngine& game_state, const std::string& filename);
    static GameEngine load_from_file(const std::string& filename);
    static std::vector<std::string> list_save_files(const std::string& directory);
private:
    static void write_int(std::ostream& out, int val);
    static void write_string(std::ostream& out, const std::string& str);
    static int read_int(std::istream& in);
    static std::string read_string(std::istream& in);
};