#include "GameSaveSystem.h"
#include "GameStateSerializer.h"
#include "FileNotFoundException.h"
#include "FileWriteException.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

GameSaveSystem::GameSaveSystem()
    : serializer(std::make_unique<GameStateSerializer>()) {
}

GameSaveSystem::GameSaveSystem(const std::string& saveFileName)
    : defaultSaveFile(saveFileName),
    serializer(std::make_unique<GameStateSerializer>()) {
}

GameSaveSystem::~GameSaveSystem() = default;

void GameSaveSystem::saveGame(const GameState& gameState) const {
    saveGame(gameState, defaultSaveFile);
}

void GameSaveSystem::saveGame(const GameState& gameState, const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw FileWriteException(filename);
    }

    try {
        std::stringstream buffer;
        serializer->serialize(gameState, buffer);

        std::string data = buffer.str();
        int checksum = calculateChecksum(data); // Вычисляем контрольную сумму

        file << data;
        file << "CHECKSUM:" << checksum << std::endl;

        if (file.fail()) {
            throw FileWriteException(filename);
        }

        std::cout << "Game saved successfully to: " << filename << std::endl;
    }
    catch (const std::exception& e) {
        throw FileWriteException(filename + " - " + e.what());
    }
}

bool GameSaveSystem::loadGame(GameState& gameState) const {
    return loadGame(gameState, defaultSaveFile);
}

bool GameSaveSystem::loadGame(GameState& gameState, const std::string& filename) const {
    if (!saveExists(filename)) {
        throw FileNotFoundException(filename);
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FileNotFoundException(filename);
    }

    try {
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string data = buffer.str();

        // Проверяем контрольную сумму на целостность данных
        if (!validateChecksum(data)) {
            std::cout << "ERROR: Save file has been modified! Loading blocked." << std::endl;
            throw SaveLoadException("Save file corrupted or invalid checksum");
        }

        buffer.seekg(0);
        serializer->deserialize(gameState, buffer);

        std::cout << "Game loaded successfully from: " << filename << std::endl;
        return true;
    }
    catch (const SaveLoadException&) {
        throw;
    }
    catch (const std::exception& e) {
        throw SaveLoadException("Failed to load game: " + std::string(e.what()));
    }
}

// Метод для расчета контрольной суммы строки данных
int GameSaveSystem::calculateChecksum(const std::string& data) const {
    int sum = 0;
    for (char c : data) {
        sum = (sum * 31 + static_cast<int>(c)) % 100000;
    }
    return sum;
}

// Метод для проверки контрольной суммы данных
bool GameSaveSystem::validateChecksum(const std::string& data) const {
    size_t checksumPos = data.find("CHECKSUM:");
    if (checksumPos == std::string::npos) {
        return false;
    }

    std::string gameData = data.substr(0, checksumPos);
    std::string checksumStr = data.substr(checksumPos + 9);

    try {
        int savedChecksum = std::stoi(checksumStr);
        int calculatedChecksum = calculateChecksum(gameData);
        return savedChecksum == calculatedChecksum;
    }
    catch (...) {
        return false;
    }
}

bool GameSaveSystem::saveExists() const {
    return saveExists(defaultSaveFile);
}

bool GameSaveSystem::saveExists(const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.good()) {
        return false;
    }

    try {
        std::string firstLine;
        std::getline(file, firstLine);
        return firstLine.find("GAME_SAVE") != std::string::npos;
    }
    catch (...) {
        return false;
    }
}

void GameSaveSystem::setDefaultSaveFile(const std::string& filename) {
    defaultSaveFile = filename;
}

std::string GameSaveSystem::getDefaultSaveFile() const {
    return defaultSaveFile;
}

bool GameSaveSystem::deleteSaveFile() const {
    return deleteSaveFile(defaultSaveFile);
}

bool GameSaveSystem::deleteSaveFile(const std::string& filename) const {
    if (std::remove(filename.c_str()) == 0) {
        std::cout << "Save file deleted: " << filename << std::endl;
        return true;
    }
    return false;
}