#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include <string>
#include <fstream>
#include <stdexcept>

class Game_controller;

class SaveSystem {
private:
    std::string save_filename;

public:
    SaveSystem(const std::string& filename = "game_save.txt");
    
    void save_game(const Game_controller& game);
    void load_game(Game_controller& game);
    bool save_exists() const;
    void delete_save();
};

#endif