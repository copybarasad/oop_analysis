#ifndef SAVELOADMANAGER_H
#define SAVELOADMANAGER_H

#include "FileController.h"
#include "GameField.h"
#include "SaveLoadException.h"
#include <string>
#include <memory>
#include <nlohmann/json.hpp>

class SaveLoadManager {
private:
    FileController fileController;
    
    std::string getSaveFileName() const;
    void validateSaveFile(const nlohmann::json& json) const;
    
public:
    SaveLoadManager();
    ~SaveLoadManager() = default;
    
    void saveGame(const std::string& filename, int level, const GameField& field) const;
    void loadGame(const std::string& filename, int& level, GameField& field);
    
    std::string getSaveFileNameFromUser() const;
};

#endif



