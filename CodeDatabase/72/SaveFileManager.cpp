//
// Created by Mac on 18.11.2025.
//

#include "SaveFileManager.h"
#include <sstream>
#include <iomanip>

SaveFileManager::SaveFileManager(const std::string& dir) : directory(dir) {}

std::vector<std::filesystem::path> SaveFileManager::getSortedSaveFiles() const {
    namespace fs = std::filesystem;

    std::vector<fs::path> saves;

    std::regex pattern(R"(^\d{4}_\d{2}_\d{2}_\d{2}_\d{2}_\d{2}_game\.bin$)");

    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            const auto filename = entry.path().filename().string();
            if (std::regex_match(filename, pattern)) {
                saves.push_back(entry.path());
            }
        }
    }

    std::sort(saves.begin(), saves.end(),
              [](const fs::path& a, const fs::path& b) {
                  return fs::last_write_time(a) > fs::last_write_time(b);
              });

    return saves;
}

std::string SaveFileManager::generateSaveFileName() const {
    auto now = std::time(nullptr);
    std::tm tm{};
    localtime_r(&now, &tm);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y_%m_%d_%H_%M_%S_game.bin");

    std::filesystem::path fullPath = std::filesystem::path(directory) / oss.str();
    return fullPath.string();
}

std::time_t SaveFileManager::getFileWriteTime(const std::filesystem::path& p) const {
    namespace fs = std::filesystem;

    auto ftime = fs::last_write_time(p);

    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
    );

    return std::chrono::system_clock::to_time_t(sctp);
}

std::string SaveFileManager::formatSaveFileDisplay(const std::filesystem::path& path) const {
    std::string filename = path.filename().string();
    std::smatch match;

    std::regex pattern(R"((\d{4})_(\d{2})_(\d{2})_(\d{2})_(\d{2})_(\d{2})_game\.bin)");

    if (std::regex_match(filename, match, pattern)) {
        return match[3].str() + "." + match[2].str() + "." + match[1].str() + " " +
               match[4].str() + ":" + match[5].str() + ":" + match[6].str();
    }

    return filename;
}

bool SaveFileManager::isValidSaveFile(const std::filesystem::path& path) const {
    if (!path.has_filename()) {
        return false;
    }

    std::string filename = path.filename().string();
    std::regex pattern(R"(^\d{4}_\d{2}_\d{2}_\d{2}_\d{2}_\d{2}_game\.bin$)");
    return std::regex_match(filename, pattern);
}