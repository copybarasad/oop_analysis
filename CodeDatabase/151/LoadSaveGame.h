#pragma once
#include <string>
#include <stdexcept>

struct SaveException : public std::runtime_error {
    SaveException(const std::string& msg) : std::runtime_error(msg) {}
};
struct LoadException : public std::runtime_error {
    LoadException(const std::string& msg) : std::runtime_error(msg) {}
};
struct FileIOException : public std::runtime_error {
    FileIOException(const std::string& msg) : std::runtime_error(msg) {}
};
struct ParseException : public std::runtime_error {
    ParseException(const std::string& msg) : std::runtime_error(msg) {}
};

class GameField;
class Player;

class LoadSaveGame {
public:
    static void save(GameField& field, Player& player, int level, const std::string& filename);

    static void load(GameField*& outField, Player*& outPlayer, int& outLevel, const std::string& filename);
};
