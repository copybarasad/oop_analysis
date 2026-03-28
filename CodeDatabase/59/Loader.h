#pragma once

#include <string>
#include <filesystem>
#include "GameData.h"
#include "json.hpp"

using json = nlohmann::json;
class Loader {
private:
    std::string saveDirectory;
    long calculateChecksum(const std::string& content);
    std::string getSaveFilePath(int slot) const;
public:
    Loader(const std::string& saveDir = "loads");

    bool saveGame(int slot, const GameData& data);

    bool loadGame(int slot, GameData& data);
    bool doSavesExist() const;
};