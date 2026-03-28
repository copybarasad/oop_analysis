#include "save_system.h"
#include "game_exceptions.h"
#include <fstream>
#include <filesystem>
#include <cstring>

namespace fs = std::filesystem;

SaveSystem::SaveSystem() = default;

void SaveSystem::saveToFile(const std::string& filename, const GameState& state) {
    try {
        validateState(state);
        
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw FileAccessException(filename, "Could not open file for writing");
        }
        
        std::string ver = state.version;
        size_t verLen = ver.length();
        file.write(reinterpret_cast<const char*>(&verLen), sizeof(verLen));
        file.write(ver.c_str(), verLen);
        
        file.write(reinterpret_cast<const char*>(&state.currentLevel), sizeof(state.currentLevel));
        file.write(reinterpret_cast<const char*>(&state.playerHealthMax), sizeof(state.playerHealthMax));
        file.write(reinterpret_cast<const char*>(&state.playerHealth), sizeof(state.playerHealth));
        file.write(reinterpret_cast<const char*>(&state.playerMeleeDamage), sizeof(state.playerMeleeDamage));
        file.write(reinterpret_cast<const char*>(&state.playerRangedDamage), sizeof(state.playerRangedDamage));
        file.write(reinterpret_cast<const char*>(&state.playerScore), sizeof(state.playerScore));
        file.write(reinterpret_cast<const char*>(&state.fieldRows), sizeof(state.fieldRows));
        file.write(reinterpret_cast<const char*>(&state.fieldCols), sizeof(state.fieldCols));
        
        size_t spellCount = state.playerSpells.size();
        file.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));
        for (const auto& spell : state.playerSpells) {
            size_t len = spell.length();
            file.write(reinterpret_cast<const char*>(&len), sizeof(len));
            file.write(spell.c_str(), len);
        }
        
        size_t entityCount = state.entityIds.size();
        file.write(reinterpret_cast<const char*>(&entityCount), sizeof(entityCount));
        for (size_t i = 0; i < entityCount; ++i) {
            file.write(reinterpret_cast<const char*>(&state.entityIds[i]), sizeof(state.entityIds[i]));
            file.write(reinterpret_cast<const char*>(&state.entityHealths[i]), sizeof(state.entityHealths[i]));
            file.write(reinterpret_cast<const char*>(&state.entityTypes[i]), sizeof(state.entityTypes[i]));
            file.write(reinterpret_cast<const char*>(&state.entityPositionsRow[i]), sizeof(state.entityPositionsRow[i]));
            file.write(reinterpret_cast<const char*>(&state.entityPositionsCols[i]), sizeof(state.entityPositionsCols[i]));
        }
        
        if (!file.good()) {
            throw SaveGameException(filename, "Error writing data to file");
        }
        
        file.close();
        if (!file) {
            throw SaveGameException(filename, "Error closing file");
        }
    } catch (const FileAccessException&) {
        throw;
    } catch (const SaveGameException&) {
        throw;
    } catch (const std::exception& e) {
        throw SaveGameException(filename, std::string("Unexpected error: ") + e.what());
    }
}

GameState SaveSystem::loadFromFile(const std::string& filename) {
    try {
        if (!fs::exists(filename)) {
            throw FileAccessException(filename, "File does not exist");
        }
        
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw FileAccessException(filename, "Could not open file for reading");
        }
        
        GameState state;
        
        size_t verLen;
        file.read(reinterpret_cast<char*>(&verLen), sizeof(verLen));
        if (file.fail() || verLen > 100) {
            throw LoadGameException(filename, "Invalid file format (version read error)");
        }
        
        std::string ver(verLen, '\0');
        file.read(&ver[0], verLen);
        if (file.fail() || ver != GameState::VERSION) {
            throw LoadGameException(filename, "Incompatible save version (expected " + std::string(GameState::VERSION) + ")");
        }
        
        file.read(reinterpret_cast<char*>(&state.currentLevel), sizeof(state.currentLevel));
        file.read(reinterpret_cast<char*>(&state.playerHealthMax), sizeof(state.playerHealthMax));
        file.read(reinterpret_cast<char*>(&state.playerHealth), sizeof(state.playerHealth));
        file.read(reinterpret_cast<char*>(&state.playerMeleeDamage), sizeof(state.playerMeleeDamage));
        file.read(reinterpret_cast<char*>(&state.playerRangedDamage), sizeof(state.playerRangedDamage));
        file.read(reinterpret_cast<char*>(&state.playerScore), sizeof(state.playerScore));
        file.read(reinterpret_cast<char*>(&state.fieldRows), sizeof(state.fieldRows));
        file.read(reinterpret_cast<char*>(&state.fieldCols), sizeof(state.fieldCols));
        
        if (file.fail()) {
            throw LoadGameException(filename, "Error reading main game data");
        }
        
        if (state.playerHealthMax <= 0 || state.playerHealthMax > 1000 ||
            state.fieldRows <= 0 || state.fieldRows > 1000 ||
            state.fieldCols <= 0 || state.fieldCols > 1000) {
            throw InvalidGameStateException("Invalid parameter values in save file");
        }
        
        size_t spellCount;
        file.read(reinterpret_cast<char*>(&spellCount), sizeof(spellCount));
        if (file.fail() || spellCount > 1000) {
            throw LoadGameException(filename, "Error reading spell count");
        }
        
        for (size_t i = 0; i < spellCount; ++i) {
            size_t len;
            file.read(reinterpret_cast<char*>(&len), sizeof(len));
            if (file.fail() || len > 1000) {
                throw LoadGameException(filename, "Error reading spell " + std::to_string(i));
            }
            std::string spell(len, '\0');
            file.read(&spell[0], len);
            if (file.fail()) {
                throw LoadGameException(filename, "Error reading spell data " + std::to_string(i));
            }
            state.playerSpells.push_back(spell);
        }
        
        size_t entityCount;
        file.read(reinterpret_cast<char*>(&entityCount), sizeof(entityCount));
        if (file.fail() || entityCount > 10000) {
            throw LoadGameException(filename, "Error reading entity count");
        }
        
        for (size_t i = 0; i < entityCount; ++i) {
            int id, health, type, row, col;
            file.read(reinterpret_cast<char*>(&id), sizeof(id));
            file.read(reinterpret_cast<char*>(&health), sizeof(health));
            file.read(reinterpret_cast<char*>(&type), sizeof(type));
            file.read(reinterpret_cast<char*>(&row), sizeof(row));
            file.read(reinterpret_cast<char*>(&col), sizeof(col));
            
            if (file.fail()) {
                throw LoadGameException(filename, "Error reading entity data " + std::to_string(i));
            }
            
            state.entityIds.push_back(id);
            state.entityHealths.push_back(health);
            state.entityTypes.push_back(type);
            state.entityPositionsRow.push_back(row);
            state.entityPositionsCols.push_back(col);
        }
        
        file.close();
        if (file.fail()) {
            throw LoadGameException(filename, "Error closing file");
        }
        
        return state;
    } catch (const FileAccessException&) {
        throw;
    } catch (const LoadGameException&) {
        throw;
    } catch (const InvalidGameStateException&) {
        throw;
    } catch (const std::exception& e) {
        throw LoadGameException(filename, std::string("Unexpected error: ") + e.what());
    }
}

bool SaveSystem::fileExists(const std::string& filename) const {
    return fs::exists(filename);
}

void SaveSystem::validateState(const GameState& state) const {
    if (state.playerHealthMax <= 0 || state.playerHealthMax > 1000) {
        throw InvalidGameStateException("Invalid playerHealthMax");
    }
    if (state.fieldRows <= 0 || state.fieldRows > 1000) {
        throw InvalidGameStateException("Invalid fieldRows");
    }
    if (state.fieldCols <= 0 || state.fieldCols > 1000) {
        throw InvalidGameStateException("Invalid fieldCols");
    }
}
