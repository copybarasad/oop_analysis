#ifndef SAVE_LOAD_MANAGER_H
#define SAVE_LOAD_MANAGER_H

#include "game_field.h"
#include "game.h"
#include "player.h"
#include "hand.h"
#include <string>
#include <fstream>
#include <vector>

class Save_load_exception : public std::exception{
private:
    std::string message;

public:
    explicit Save_load_exception(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class File_not_found_exception : public Save_load_exception{
public:
    File_not_found_exception() : Save_load_exception("Файл сохранения не найден") {}
};

class Injured_file_exception : public Save_load_exception{
public:
    Injured_file_exception() : Save_load_exception("Файл сохранения поврежден") {}
};

class Save_load_manager{
private:
    class FileHandler {
    private:
        std::fstream file;
        std::string file_name;
    public:
        FileHandler(const std::string& name, std::ios::openmode mode);
        ~FileHandler();  
        
        std::fstream& get() { return file; }
        bool is_open() const { return file.is_open(); }
        bool good() const { return file.good(); }
    };

public:
    static void save_game(const Game& game, const std::string& file_name);
    static Game load_game(const std::string& file_name);
    static bool save_exists(const std::string& file_name);

private:
    static void save_player(std::fstream& file, const Player& player);
    static void load_player(std::fstream& file, Player& player);
    static void save_field(std::fstream& file, const Game_field& field);
    static void load_field(std::fstream& file, Game_field& field);
};

#endif