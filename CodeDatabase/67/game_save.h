#ifndef GAME_SAVE_H
#define GAME_SAVE_H

#include <string>
#include <fstream>
#include <stdexcept>

class game;

class GameSave {
public:
    GameSave() = default;
    void saveGame(const game* game_ptr, const std::string& filename) const;
    
    void loadGame(game* game_ptr, const std::string& filename);
    
private:
    std::string trim(const std::string& str) const;
    std::string getValue(const std::string& line) const;
};

class SaveGameException : public std::runtime_error {
public:
    explicit SaveGameException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif
