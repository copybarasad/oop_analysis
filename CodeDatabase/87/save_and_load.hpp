#ifndef SAVE_AND_LOAD_HPP
#define SAVE_AND_LOAD_HPP
#include <memory>
#include <string>

class Game_controller;
class Save_and_load{
    private:
        std::string escape_string(const std::string& s);
        std::string unescape_string(std::string& s);
        int parse_int(const std::string& s, const std::string& field_name);
        void validate_directory(std::string& dir_path);
    public:
        void save_game(Game_controller& game, const std::string& filename);
        std::unique_ptr<Game_controller> load_game(const std::string& filename);
};

#endif