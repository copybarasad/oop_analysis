//
// Created by Mac on 18.11.2025.
//

#ifndef GAME_TERM_SAVEFILEMANAGER_H
#define GAME_TERM_SAVEFILEMANAGER_H
#include <filesystem>
#include <regex>
#include <string>


class SaveFileManager {
    std::string directory;

public:
    explicit SaveFileManager(const std::string& dir);

    std::vector<std::filesystem::path> getSortedSaveFiles() const;
    std::string generateSaveFileName() const;
    std::time_t getFileWriteTime(const std::filesystem::path& p) const;
    std::string formatSaveFileDisplay(const std::filesystem::path& path) const;

private:
    bool isValidSaveFile(const std::filesystem::path& path) const;
};




#endif //GAME_TERM_SAVEFILEMANAGER_H