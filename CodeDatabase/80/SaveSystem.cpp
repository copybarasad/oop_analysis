#include "SaveSystem.h"
#include <iostream>
#include <filesystem>
#include <sstream>
#include <system_error>

#ifdef _WIN32
#include <direct.h>
#define CREATE_DIR(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#define CREATE_DIR(dir) mkdir(dir, 0755)
#endif

class FileRAII {
private:
    std::fstream file;

public:
    FileRAII(const std::string& filename, std::ios_base::openmode mode) {
        file.open(filename, mode);
    }

    ~FileRAII() {
        if (file.is_open()) {
            file.close();
        }
    }

    std::fstream& get() { return file; }
    bool is_open() const { return file.is_open(); }
};

class TempFileCleaner {
private:
    std::string filename;
    bool committed;

public:
    TempFileCleaner(const std::string& fname) : filename(fname), committed(false) {}

    ~TempFileCleaner() {
        if (!committed && !filename.empty()) {
            try {
                if (std::filesystem::exists(filename)) {
                    std::filesystem::remove(filename);
                }
            }
            catch (...) {
            }
        }
    }

    void commit() { committed = true; }
};

class SaveException : public std::exception {
private:
    std::string message;
public:
    SaveException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class LoadException : public std::exception {
private:
    std::string message;
public:
    LoadException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

SaveSystem::SaveSystem() : saveDirectory("saves") {
    try {
        CREATE_DIR(saveDirectory.c_str());
        if (!std::filesystem::exists(saveDirectory)) {
            throw SaveException("Cannot create save directory: " + saveDirectory);
        }
    }
    catch (const std::exception& e) {
        std::cout << "Warning: " << e.what() << std::endl;
    }
}

bool SaveSystem::SaveGame(const std::string& saveName, Map& map, Player& player,
    Tower& tower, AttackTower& attackTower, int counter, int currentLevel) {

    if (saveName.empty() || saveName.find_first_of("\\/:*?\"<>|") != std::string::npos) {
        std::cout << "Error: Invalid save name! Contains forbidden characters." << std::endl;
        return false;
    }

    std::string filename = saveDirectory + "/" + saveName + ".sav";
    std::string tempFilename = filename + ".tmp";
    TempFileCleaner tempCleaner(tempFilename);

    try {
        if (!std::filesystem::exists(saveDirectory)) {
            throw SaveException("Save directory does not exist: " + saveDirectory);
        }

        auto space = std::filesystem::space(saveDirectory);
        if (space.available < 1024 * 1024) {
            throw SaveException("Not enough disk space for saving game");
        }

        FileRAII fileRAII(tempFilename, std::ios::binary | std::ios::out);
        auto& file = fileRAII.get();

        if (!file.is_open()) {
            throw SaveException("Cannot create or open save file: " + tempFilename);
        }

        file << "test";
        if (file.fail()) {
            throw SaveException("No write permissions for save file: " + tempFilename);
        }
        file.seekp(0);

        file << "=== GAME SAVE ===" << std::endl;
        file << "Version:1.1" << std::endl;
        file << "Counter:" << counter << std::endl;
        file << "CurrentLevel:" << currentLevel << std::endl << std::endl;

        std::string mapData = map.SaveToString();
        if (mapData.empty()) {
            throw SaveException("Failed to serialize map data");
        }
        file << "=== " << map.GetMapHeader() << " ===" << std::endl;
        file << mapData << std::endl;

        std::string playerData = player.SaveToString();
        if (playerData.empty()) {
            throw SaveException("Failed to serialize player data");
        }
        file << "=== " << player.GetSaveHeader() << " ===" << std::endl;
        file << playerData << std::endl;

        std::string towerData = tower.SaveToString();
        if (towerData.empty()) {
            throw SaveException("Failed to serialize tower data");
        }
        file << "=== " << tower.GetSaveHeader() << " ===" << std::endl;
        file << towerData << std::endl;

        std::string attackTowerData = attackTower.SaveToString();
        if (attackTowerData.empty()) {
            throw SaveException("Failed to serialize attack tower data");
        }
        file << "=== " << attackTower.GetSaveHeader() << " ===" << std::endl;
        file << attackTowerData << std::endl;

        file << "=== ENEMIES ===" << std::endl;
        auto& objects = map.GetObjects();
        int enemyCount = 0;

        for (auto& [coord, entity] : objects) {
            if (entity && entity->ind_type == 'e') {
                std::string enemyData = entity->SaveToString();
                if (enemyData.empty()) {
                    std::cout << "Warning: Failed to serialize enemy at " << coord.first << "," << coord.second << std::endl;
                    continue;
                }
                file << "--- Enemy " << enemyCount << " ---" << std::endl;
                file << enemyData << std::endl;
                enemyCount++;
            }
        }
        file << "TotalEnemies:" << enemyCount << std::endl;

        if (file.fail()) {
            throw SaveException("Write operation failed during saving");
        }

        file.close();

        if (std::filesystem::exists(filename)) {
            std::filesystem::remove(filename);
        }
        std::filesystem::rename(tempFilename, filename);
        tempCleaner.commit();

        if (!std::filesystem::exists(filename) || std::filesystem::file_size(filename) == 0) {
            throw SaveException("Save file creation failed - file is empty or doesn't exist");
        }

        std::cout << "Game saved successfully: " << saveName
            << " (level: " << currentLevel << ", enemies: " << enemyCount << ")" << std::endl;
        return true;

    }
    catch (const SaveException& e) {
        std::cout << "Save Error: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception& e) {
        std::cout << "Unexpected error during saving: " << e.what() << std::endl;
        return false;
    }
}

bool SaveSystem::LoadGame(const std::string& saveName, Map& map, Player& player,
    Tower& tower, AttackTower& attackTower, int& counter, int& currentLevel) {

    std::string filename = saveDirectory + "/" + saveName + ".sav";

    try {
        if (!std::filesystem::exists(filename)) {
            throw LoadException("Save file not found: " + filename);
        }

        auto fileSize = std::filesystem::file_size(filename);
        if (fileSize == 0) {
            throw LoadException("Save file is empty: " + filename);
        }
        if (fileSize > 10 * 1024 * 1024) {
            throw LoadException("Save file is too large: " + filename);
        }

        FileRAII fileRAII(filename, std::ios::in);
        auto& file = fileRAII.get();

        if (!file.is_open()) {
            throw LoadException("Cannot open save file: " + filename);
        }

        std::string firstLine;
        if (!std::getline(file, firstLine) || firstLine.find("=== GAME SAVE ===") == std::string::npos) {
            throw LoadException("Invalid save file format: missing header");
        }

        map.ClearAllObjects();

        std::string line;
        std::string currentSection;
        std::stringstream sectionData;
        int enemyCount = 0;
        int expectedEnemies = 0;
        bool versionChecked = false;
        currentLevel = 1;

        while (std::getline(file, line)) {
            if (line.empty()) continue;

            if (!versionChecked && line.find("Version:") != std::string::npos) {
                std::string version = line.substr(line.find(':') + 1);
                if (version != "1.0" && version != "1.1") {
                    std::cout << "Warning: Save file version mismatch. Expected 1.0 or 1.1, got " << version << std::endl;
                }
                versionChecked = true;
                continue;
            }

            if (line.find("Counter:") != std::string::npos) {
                counter = std::stoi(line.substr(line.find(':') + 1));
                continue;
            }

            if (line.find("CurrentLevel:") != std::string::npos) {
                currentLevel = std::stoi(line.substr(line.find(':') + 1));
                continue;
            }

            if (line.find("===") != std::string::npos) {
                if (!currentSection.empty()) {
                    ProcessSection(currentSection, sectionData.str(), map, player, tower, attackTower, counter, enemyCount, currentLevel);
                }

                currentSection = line;
                sectionData.str("");
                sectionData.clear();
                continue;
            }

            if (line.find("--- Enemy") != std::string::npos) {
                if (currentSection.find("ENEMIES") != std::string::npos) {
                    if (!sectionData.str().empty()) {
                        ProcessEnemySection(sectionData.str(), map, enemyCount);
                        enemyCount++;
                    }
                    sectionData.str("");
                    sectionData.clear();
                }
                continue;
            }

            if (line.find("TotalEnemies:") != std::string::npos) {
                expectedEnemies = std::stoi(line.substr(line.find(':') + 1));
                continue;
            }

            sectionData << line << "\n";
        }

        if (!currentSection.empty() && !sectionData.str().empty()) {
            ProcessSection(currentSection, sectionData.str(), map, player, tower, attackTower, counter, enemyCount, currentLevel);
        }

        if (!sectionData.str().empty() && currentSection.find("ENEMIES") != std::string::npos) {
            ProcessEnemySection(sectionData.str(), map, enemyCount);
            enemyCount++;
        }

        if (expectedEnemies > 0 && enemyCount != expectedEnemies) {
            std::cout << "Warning: Enemy count mismatch. Expected " << expectedEnemies << ", loaded " << enemyCount << std::endl;
        }

        map.SetVal(player.x, player.y, player.ind_type, &player);
        map.SetVal(tower.x, tower.y, tower.ind_type, &tower);
        map.SetVal(attackTower.x, attackTower.y, attackTower.ind_type, &attackTower);

        std::cout << "Game loaded successfully: " << saveName << std::endl;
        std::cout << "Level: " << currentLevel << ", enemies: " << enemyCount << ", counter: " << counter << std::endl;
        return true;

    }
    catch (const LoadException& e) {
        std::cout << "Load Error: " << e.what() << std::endl;
        map.ClearAllObjects();
        return false;
    }
    catch (const std::exception& e) {
        std::cout << "Unexpected error during loading: " << e.what() << std::endl;
        map.ClearAllObjects();
        return false;
    }
}

void SaveSystem::ProcessSection(const std::string& section, const std::string& data,
    Map& map, Player& player, Tower& tower,
    AttackTower& attackTower, int& counter, int& enemyCount, int& currentLevel) {

    try {
        std::cout << "DEBUG - Processing section: " << section << std::endl;

        if (section == "=== ATTACK_TOWER ===") {
            std::cout << "DEBUG - Loading ATTACK_TOWER data" << std::endl;
            std::cout << "DEBUG - AttackTower data: " << data << std::endl;
            attackTower.LoadFromString(data);
        }
        else if (section == "=== TOWER ===") {
            std::cout << "DEBUG - Loading TOWER data" << std::endl;
            tower.LoadFromString(data);
        }
        else if (section == "=== PLAYER ===") {
            std::cout << "DEBUG - Loading PLAYER data" << std::endl;
            player.LoadFromString(data);
        }
        else if (section == "=== MAP ===") {
            std::cout << "DEBUG - Loading MAP data" << std::endl;
            map.LoadFromString(data);
        }
        else if (section.find("ENEMIES") != std::string::npos) {
            std::cout << "DEBUG - Processing ENEMIES section" << std::endl;
        }
        else {
            std::cout << "DEBUG - Unknown section: " << section << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error processing section " << section << ": " << e.what() << std::endl;
        throw;
    }
}

void SaveSystem::ProcessEnemySection(const std::string& data, Map& map, int enemyIndex) {
    try {
        if (data.empty()) {
            std::cout << "Warning: Empty data for enemy " << enemyIndex << std::endl;
            return;
        }

        Enemy* enemy = new Enemy();
        enemy->LoadFromString(data);

        if (enemy->x < 0 || enemy->y < 0 || enemy->heallpoints <= 0) {
            std::cout << "Warning: Invalid enemy data at index " << enemyIndex << ", skipping" << std::endl;
            delete enemy;
            return;
        }

        if (enemy->x >= map.GetHeight() || enemy->y >= map.GetWeight()) {
            std::cout << "Warning: Enemy coordinates out of bounds at index " << enemyIndex << ", skipping" << std::endl;
            delete enemy;
            return;
        }

        auto& objects = map.GetObjects();
        auto existing = objects.find({ enemy->x, enemy->y });
        if (existing != objects.end() && existing->second != nullptr) {
            delete existing->second;
            objects.erase(existing);
        }

        map.SetVal(enemy->x, enemy->y, enemy->ind_type, enemy);
    }
    catch (const std::exception& e) {
        std::cout << "Error loading enemy " << enemyIndex << ": " << e.what() << std::endl;
    }
}

std::vector<std::string> SaveSystem::GetSaveList() {
    std::vector<std::string> saves;

    try {
        if (!std::filesystem::exists(saveDirectory)) {
            std::cout << "Save directory not found: " << saveDirectory << std::endl;
            return saves;
        }

        for (const auto& entry : std::filesystem::directory_iterator(saveDirectory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".sav") {
                if (entry.file_size() > 0) {
                    saves.push_back(entry.path().stem().string());
                }
            }
        }

        std::sort(saves.begin(), saves.end(), [this](const std::string& a, const std::string& b) {
            auto timeA = std::filesystem::last_write_time(saveDirectory + "/" + a + ".sav");
            auto timeB = std::filesystem::last_write_time(saveDirectory + "/" + b + ".sav");
            return timeA > timeB;
            });

    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cout << "Error accessing save directory: " << e.what() << std::endl;
    }

    return saves;
}

bool SaveSystem::SaveExists(const std::string& saveName) {
    try {
        std::string filename = saveDirectory + "/" + saveName + ".sav";
        return std::filesystem::exists(filename) && std::filesystem::file_size(filename) > 0;
    }
    catch (...) {
        return false;
    }
}

void SaveSystem::DeleteSave(const std::string& saveName) {
    try {
        std::string filename = saveDirectory + "/" + saveName + ".sav";
        if (std::filesystem::exists(filename)) {
            if (std::filesystem::remove(filename)) {
                std::cout << "Save deleted: " << saveName << std::endl;
            }
            else {
                std::cout << "Failed to delete save: " << saveName << " (file might be in use)" << std::endl;
            }
        }
        else {
            std::cout << "Save not found: " << saveName << std::endl;
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cout << "Error deleting save " << saveName << ": " << e.what() << std::endl;
    }
}