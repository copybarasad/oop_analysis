#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include <string>
#include "game_state.h"

class SaveSystem {
public:
    SaveSystem();
    
    void saveToFile(const std::string& filename, const GameState& state);
    GameState loadFromFile(const std::string& filename);
    bool fileExists(const std::string& filename) const;

private:
    void validateState(const GameState& state) const;
};

#endif
