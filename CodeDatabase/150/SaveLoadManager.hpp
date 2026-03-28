#ifndef SAVELOADMANAGER_H
#define SAVELOADMANAGER_H

#include "GameState.hpp"
#include "GameExceptions.hpp"
#include "FileRAII.hpp"
#include <string>
#include <fstream>

class SaveLoadManager {
private:
    std::string saveFileName;

public:
    SaveLoadManager(const std::string& filename);
    
    void saveGame(const GameState& state) const;
    std::unique_ptr<GameState> loadGame();
    
    void serializeGame(std::ostream& os, const GameState& state) const;
    void deserializeGame(std::istream& is, GameState& state);
    
    const std::string& getSaveFileName() const { return saveFileName; }
    void setSaveFileName(const std::string& filename) { saveFileName = filename; }
};

#endif