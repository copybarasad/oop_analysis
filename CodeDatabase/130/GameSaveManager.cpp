#include "GameSaveManager.h"
#include "GameExceptions.h"
#include <cstdio>
#include <sstream>
#include <iterator>
#include <string>
#include <cstdint>
 
namespace {
uint32_t fnv1a(const std::string& data) {
    const uint32_t prime = 16777619u;
    uint32_t hash = 2166136261u;
    for (unsigned char c : data) {
        hash ^= c;
        hash *= prime;
    }
    return hash;
}

std::string readStream(std::istream& in) {
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}
}
 
FileGuard::FileGuard(const std::string& path, std::ios::openmode mode)
    : filename_(path) {
    file_.open(path, mode);
    if (!file_.is_open()) {
        if (mode & std::ios::in) {
            throw FileNotFoundError(path);
        } else {
            throw FileWriteError(path);
        }
    }
}
 
FileGuard::~FileGuard() {
    if (file_.is_open()) {
        file_.close();
    }
}
 
void GameSaveManager::saveGame(const GameState& state) {
    std::stringstream buffer;
    state.serialize(buffer);
    std::string body = buffer.str();
    uint32_t checksum = fnv1a(body);
    FileGuard file(SAVE_FILE, std::ios::out);
    file.get() << "CHECKSUM " << checksum << "\n" << body;
}
 
GameState GameSaveManager::loadGame() {
    FileGuard file(SAVE_FILE, std::ios::in);
    std::istream& in = file.get();
    std::string firstLine;
    if (!std::getline(in, firstLine)) {
        throw CorruptedDataError("Empty save file");
    }
    if (firstLine.rfind("CHECKSUM ", 0) == 0) {
        uint32_t stored = 0;
        try {
            stored = static_cast<uint32_t>(std::stoul(firstLine.substr(9)));
        } catch (...) {
            throw CorruptedDataError("Invalid checksum header");
        }
        std::string body = readStream(in);
        uint32_t calculated = fnv1a(body);
        if (stored != calculated) {
            throw SaveIntegrityError("Save file was modified");
        }
        std::istringstream bodyStream(body);
        return GameState::deserialize(bodyStream);
    } else {
        std::string body = firstLine + "\n" + readStream(in);
        std::istringstream bodyStream(body);
        return GameState::deserialize(bodyStream);
    }
}
 
void GameSaveManager::autoSave(const GameState& state) {
    std::stringstream buffer;
    state.serialize(buffer);
    std::string body = buffer.str();
    uint32_t checksum = fnv1a(body);
    FileGuard file(AUTO_SAVE_FILE, std::ios::out);
    file.get() << "CHECKSUM " << checksum << "\n" << body;
}
 
GameState GameSaveManager::loadAutoSave() {
    FileGuard file(AUTO_SAVE_FILE, std::ios::in);
    std::istream& in = file.get();
    std::string firstLine;
    if (!std::getline(in, firstLine)) {
        throw CorruptedDataError("Empty auto-save file");
    }
    if (firstLine.rfind("CHECKSUM ", 0) == 0) {
        uint32_t stored = 0;
        try {
            stored = static_cast<uint32_t>(std::stoul(firstLine.substr(9)));
        } catch (...) {
            throw CorruptedDataError("Invalid checksum header");
        }
        std::string body = readStream(in);
        uint32_t calculated = fnv1a(body);
        if (stored != calculated) {
            throw SaveIntegrityError("Auto-save file was modified");
        }
        std::istringstream bodyStream(body);
        return GameState::deserialize(bodyStream);
    } else {
        std::string body = firstLine + "\n" + readStream(in);
        std::istringstream bodyStream(body);
        return GameState::deserialize(bodyStream);
    }
}

bool GameSaveManager::hasSaveFile() const {
    std::ifstream file(SAVE_FILE);
    return file.good();
}

bool GameSaveManager::hasAutoSave() const {
    std::ifstream file(AUTO_SAVE_FILE);
    return file.good();
}

void GameSaveManager::deleteSaveFile() {
    std::remove(SAVE_FILE);
}

void GameSaveManager::deleteAutoSave() {
    std::remove(AUTO_SAVE_FILE);
}

void GameSaveManager::deleteAllSaves() {
    deleteSaveFile();
    deleteAutoSave();
}

