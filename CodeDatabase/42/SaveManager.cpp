#include "SaveManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <chrono>
#include <ctime>

namespace fs = std::filesystem;

SaveManager::SaveManager() {
    try {
        if (!fs::exists("saves")) {
            fs::create_directory("saves");
        }
        
        if (!fs::exists("saves/backups")) {
            fs::create_directory("saves/backups");
        }
    } catch (const std::exception& e) {
        std::cerr << "Warning: Cannot create save directories: " << e.what() << std::endl;
    }
}

void SaveManager::restoreTowerState(EnemyTower& tower, const SavedTower& savedTower) const {
    int damageTaken = savedTower.maxHealth - savedTower.health;
    if (damageTaken > 0) {
        tower.takeDamageInternal(damageTaken);
    }
    
    tower.setCooldownInternal(savedTower.cooldown);
}

void SaveManager::validateSaveSlot(int slot) const {
    if (slot < 1 || slot > MAX_SAVE_SLOTS) {
        throw SaveLoadException("Invalid save slot: " + std::to_string(slot));
    }
}

std::string SaveManager::getSlotFileName(int slot) {
    std::ostringstream oss;
    oss << "saves/save" << slot << ".dat";
    return oss.str();
}

std::string SaveManager::getSlotBackupFileName(int slot) {
    std::ostringstream oss;
    oss << "saves/backups/save" << slot << ".bak";
    return oss.str();
}

bool SaveManager::saveExists(int slot) const {
    validateSaveSlot(slot);
    std::string filename = getSlotFileName(slot);
    try {
        return fs::exists(filename);
    } catch (const std::exception& e) {
        throw SaveLoadException("Cannot check if save exists: " + std::string(e.what()), filename);
    }
}

void SaveManager::createBackup(const std::string& filename) const {
    try {
        if (fs::exists(filename)) {
            std::string backupName = filename;
            size_t pos = backupName.find_last_of('/');
            if (pos != std::string::npos) {
                backupName = "saves/backups/" + backupName.substr(pos + 1) + ".bak";
            }
            fs::copy_file(filename, backupName, fs::copy_options::overwrite_existing);
        }
    } catch (const std::exception& e) {
        std::cerr << "Warning: Cannot create backup: " << e.what() << std::endl;
    }
}

void SaveManager::restoreBackup(const std::string& filename) const {
    try {
        std::string backupName = filename;
        size_t pos = backupName.find_last_of('/');
        if (pos != std::string::npos) {
            backupName = "saves/backups/" + backupName.substr(pos + 1) + ".bak";
        }
        
        if (fs::exists(backupName)) {
            fs::copy_file(backupName, filename, fs::copy_options::overwrite_existing);
        }
    } catch (const std::exception& e) {
        std::cerr << "Warning: Cannot restore backup: " << e.what() << std::endl;
    }
}

bool SaveManager::writeToFile(const SaveData& data, const std::string& filename) {
    try {
        createBackup(filename);
        
        std::string tempFilename = filename + ".tmp";
        std::ofstream file(tempFilename, std::ios::binary);
        
        if (!file) {
            throw SaveLoadException("Cannot open file for writing", filename);
        }
        
        size_t dataSize = data.getSerializedSize();
        std::vector<char> buffer(dataSize);
        data.serialize(buffer.data());
        
        file.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
        file.write(buffer.data(), buffer.size());
        
        if (!file.good()) {
            throw SaveLoadException("Failed to write data to file", filename);
        }
        
        file.close();
        
        fs::rename(tempFilename, filename);
        
        return true;
        
    } catch (const std::exception& e) {
        restoreBackup(filename);
        throw;
    }
}

bool SaveManager::readFromFile(SaveData& data, const std::string& filename) const {
    try {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        
        if (!file) {
            throw SaveLoadException("Cannot open file for reading", filename);
        }
        
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        
        if (fileSize < static_cast<std::streamsize>(sizeof(size_t))) {
            throw SaveLoadException("File is too small or corrupted", filename);
        }
        
        size_t dataSize;
        file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
        
        if (dataSize > static_cast<size_t>(fileSize) - sizeof(dataSize)) {
            throw SaveLoadException("Data size mismatch, file may be corrupted", filename);
        }
        
        std::vector<char> buffer(dataSize);
        file.read(buffer.data(), dataSize);
        
        if (!file.good()) {
            throw SaveLoadException("Failed to read data from file", filename);
        }
        
        if (!data.deserialize(buffer.data(), dataSize)) {
            throw SaveLoadException("Failed to deserialize data, file may be corrupted", filename);
        }
        
        return true;
        
    } catch (const std::exception& e) {
        throw;
    }
}

bool SaveManager::saveGame(const SaveData& data, int slot) {
    try {
        validateSaveSlot(slot);
        
        std::string filename = getSlotFileName(slot);
        bool success = writeToFile(data, filename);
        
        return success;
        
    } catch (const SaveLoadException& e) {
        std::cerr << "Error saving game: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error saving game: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::loadGame(SaveData& data, int slot) {
    try {
        validateSaveSlot(slot);
        
        std::string filename = getSlotFileName(slot);
        
        if (!saveExists(slot)) {
            throw SaveLoadException("Save file does not exist", filename);
        }
        
        bool success = readFromFile(data, filename);
        
        return success;
        
    } catch (const SaveLoadException& e) {
        std::cerr << "Error loading game: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error loading game: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::deleteSave(int slot) {
    try {
        validateSaveSlot(slot);
        
        std::string filename = getSlotFileName(slot);
        if (fs::exists(filename)) {
            bool removed = fs::remove(filename);
            if (removed) {
                return true;
            }
            return false;
        }
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error deleting save: " << e.what() << std::endl;
        return false;
    }
}


std::vector<int> SaveManager::getAvailableSaves() const {
    std::vector<int> available;
    for (int i = 1; i <= MAX_SAVE_SLOTS; i++) {
        try {
            if (saveExists(i)) {
                available.push_back(i);
            }
        } catch (const std::exception& e) {
            std::cerr << "Warning checking save slot " << i << ": " << e.what() << std::endl;
        }
    }
    return available;
}

std::string SaveManager::getSaveInfo(int slot) const {
    try {
        if (!saveExists(slot)) {
            return "Empty slot";
        }
        
        SaveData data;
        std::string filename = getSlotFileName(slot);
        
        if (!readFromFile(data, filename)) {
            return "Corrupted save";
        }
        
        std::ostringstream oss;
        oss << "Health: " << data.playerHealth << "/" << data.playerMaxHealth
            << " | Turn: " << data.turnCount
            << " | Enemies: " << data.enemies.size()
            << " | Towers: " << data.towers.size()
            << " | Spells: " << data.spells.size();
        
        return oss.str();
        
    } catch (const std::exception& e) {
        return "Error: " + std::string(e.what());
    }
}