#include "SaveManager.hpp"
#include "GameCycle.hpp"
#include <iostream>
#include <fstream>

SaveManager::SaveManager() {
    refreshSaveSlots();
}

bool SaveManager::saveExists(const std::string& filename) const {
    return fs::exists(filename);
}

std::string SaveManager::getSaveFilename(int slot) const {
    if (slot < 0 || slot > MAX_SAVE_SLOTS) {
        return "savegame_1.dat";
    }

    if (slot == 0) {
        return "autosave.dat"; // Слот 0 для автосохранения
    }

    return "savegame_" + std::to_string(slot) + ".dat";
}

bool SaveManager::saveGame(int slot, const GameCycle& game) const {
    std::string filename = getSaveFilename(slot);

    try {
        if (slot < 1 || slot > MAX_SAVE_SLOTS) {
            throw std::runtime_error("Invalid save slot number: " + std::to_string(slot));
        }

        std::ofstream testFile(filename, std::ios::app);
        if (!testFile.is_open()) {
            throw std::runtime_error("Cannot write to file - permission denied or directory doesn't exist: " + filename);
        }
        testFile.close();

        // Вызов сохранения игры
        game.saveGame(filename);

        // Проверяем не пустой
        if (!fs::exists(filename)) {
            throw std::runtime_error("Save file was not created: " + filename);
        }

        std::ifstream checkFile(filename, std::ios::binary | std::ios::ate);
        if (!checkFile.is_open()) {
            throw std::runtime_error("Cannot verify saved file: " + filename);
        }

        std::streamsize size = checkFile.tellg();
        checkFile.close();

        if (size == 0) {
            throw std::runtime_error("Save file is empty after saving: " + filename);
        }

        // Обновляем информацию о слотах
        const_cast<SaveManager*>(this)->refreshSaveSlots();

        std::cout << "Game successfully saved to slot " << slot << " (size: " << size << " bytes)" << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "SAVE FAILED: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::loadGame(int slot, GameCycle& game) const {
    std::string filename = getSaveFilename(slot);

    try {
        // Проверка корректности номера слота
        if (slot < 1 || slot > MAX_SAVE_SLOTS) {
            throw std::runtime_error("Invalid save slot number: " + std::to_string(slot));
        }

        // Проверка существования файла
        if (!fs::exists(filename)) {
            throw std::runtime_error("Save file does not exist: " + filename);
        }

        std::ifstream testFile(filename, std::ios::binary | std::ios::ate);
        if (!testFile.is_open()) {
            throw std::runtime_error("Cannot open save file: " + filename);
        }

        std::streamsize fileSize = testFile.tellg();
        testFile.close();

        if (fileSize == 0) {
            throw std::runtime_error("Save file is empty or corrupted: " + filename);
        }

        if (fileSize > 10 * 1024 * 1024) {
            throw std::runtime_error("Save file is too large, possibly corrupted: " + filename);
        }


        std::ifstream headerFile(filename, std::ios::binary);
        if (!headerFile.is_open()) {
            throw std::runtime_error("Cannot open save file for header check: " + filename);
        }

        char signature[5] = {0};
        headerFile.read(signature, 4);

        if (std::string(signature) != "OOPG") {
            headerFile.close();
            throw std::runtime_error("Invalid save file format - not an OOP Game save file");
        }

        int file_version;
        headerFile.read(reinterpret_cast<char*>(&file_version), sizeof(file_version));
        headerFile.close();

        if (file_version != 1) {
            throw std::runtime_error("Incompatible save file version: " + std::to_string(file_version) + " (expected: 1)");
        }

        // Загрузка игры
        game.loadGame(filename);

        std::cout << "Game successfully loaded from slot " << slot << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "LOAD FAILED: " << e.what() << std::endl;

        // Автоматически удаляем поврежденный файл
        try {
            if (fs::exists(filename)) {
                std::cerr << "Deleting corrupted save file: " << filename << std::endl;
                fs::remove(filename);
            }
        } catch (const std::exception& deleteError) {
            std::cerr << "Warning: Could not delete corrupted save file: " << deleteError.what() << std::endl;
        }

        std::cout << "Available save slots:" << std::endl;
        for (const auto& saveSlot : saveSlots) {
            if (saveSlot.exists) {
                std::cout << "  " << saveSlot.displayName << " - Level " << saveSlot.level << std::endl;
            }
        }

        return false;
    }
}

void SaveManager::refreshSaveSlots() {
    saveSlots.clear();

    // Слот для автосохранения
    SaveSlot autosaveSlot;
    autosaveSlot.filename = "autosave.dat";
    autosaveSlot.displayName = "Autosave";
    autosaveSlot.exists = fs::exists(autosaveSlot.filename);
    autosaveSlot.level = 1;

    if (autosaveSlot.exists) {
        try {
            std::ifstream file(autosaveSlot.filename, std::ios::binary);
            if (file.is_open()) {
                char signature[5] = {0};
                file.read(signature, 4);

                if (std::string(signature) == "OOPG") {
                    int version;
                    file.read(reinterpret_cast<char*>(&version), sizeof(version));
                    file.seekg(4 + sizeof(version) + sizeof(bool) * 4 + sizeof(int), std::ios::beg);

                    int level;
                    if (file.read(reinterpret_cast<char*>(&level), sizeof(level))) {
                        autosaveSlot.level = level;
                    }
                }
                file.close();
            }
        } catch (...) {
            std::cerr << "Warning: Could not read level for autosave" << std::endl;
        }
    }
    saveSlots.push_back(autosaveSlot);

    // Обычные слоты сохранения (только 1 слот)
    for (int i = 1; i <= MAX_SAVE_SLOTS; ++i) {
        SaveSlot slot;
        slot.filename = "savegame_" + std::to_string(i) + ".dat";
        slot.displayName = "Save Slot " + std::to_string(i);
        slot.exists = fs::exists(slot.filename);
        slot.level = 1;

        if (slot.exists) {
            try {
                std::ifstream file(slot.filename, std::ios::binary);
                if (file.is_open()) {
                    char signature[5] = {0};
                    file.read(signature, 4);

                    if (std::string(signature) == "OOPG") {
                        int version;
                        file.read(reinterpret_cast<char*>(&version), sizeof(version));
                        file.seekg(4 + sizeof(version) + sizeof(bool) * 4 + sizeof(int), std::ios::beg);

                        int level;
                        if (file.read(reinterpret_cast<char*>(&level), sizeof(level))) {
                            slot.level = level;
                        }
                    }
                    file.close();
                }
            } catch (...) {
                std::cerr << "Warning: Could not read level for save slot " << i << std::endl;
            }
        }

        saveSlots.push_back(slot);
    }
}

bool SaveManager::deleteSave(int slot) {
    if (slot < 1 || slot > MAX_SAVE_SLOTS) {
        return false;
    }
    
    std::string filename = getSaveFilename(slot);
    if (fs::exists(filename)) {
        return fs::remove(filename);
    }
    return false;
}