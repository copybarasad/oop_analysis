#ifndef OOP_SAVE_MANAGER_H_
#define OOP_SAVE_MANAGER_H_

#include "game_serializer.h"
#include <fstream>
#include <string>
#include <memory>

namespace rpg {

class Game;

class FileGuard {
public:
    explicit FileGuard(const std::string& filename, std::ios::openmode mode);
    ~FileGuard();

    FileGuard(const FileGuard&) = delete;
    FileGuard& operator=(const FileGuard&) = delete;

    [[nodiscard]] std::fstream& GetStream();
    [[nodiscard]] bool IsOpen() const;

private:
    std::fstream file_;
    std::string filename_;
};

class SaveManager final {
public:
    static constexpr const char* kDefaultSaveFile = "savegame.dat";

    SaveManager() = default;

    void SaveGame(const Game& game, const std::string& filename = kDefaultSaveFile);

    void LoadGame(Game& game, const std::string& filename = kDefaultSaveFile);

    [[nodiscard]] static bool SaveFileExists(const std::string& filename = kDefaultSaveFile);

private:
    GameSerializer serializer_;
};

}

#endif


