#pragma once
#include <string>
#include <fstream>
#include "general_manager.h"

class SaveSystem {
private:
    std::string filename;

public:
    SaveSystem(const std::string& file = "savegame.dat");
    void setName(const std::string& name);
    void saveGame(GeneralManager& manager, int level);
    bool loadGame(GeneralManager& manager, int& level);
    bool saveExists() const;
};