#ifndef SAVE_LOAD_MANAGER_H
#define SAVE_LOAD_MANAGER_H

#include "world.h"
#include <string>
#include <stdexcept>

class SaveLoadException : public std::runtime_error {
public:
    SaveLoadException(const std::string& msg) : std::runtime_error(msg) {}
};

class SaveLoadManager {
public:
    SaveLoadManager() = default;
    ~SaveLoadManager() = default;

    void SaveGame(const World& world, const std::string& filename);
    World LoadGame(const std::string& filename);

private:
    void SerializeWorld(const World& world, std::ofstream& file);
    World DeserializeWorld(std::ifstream& file);
};

#endif
