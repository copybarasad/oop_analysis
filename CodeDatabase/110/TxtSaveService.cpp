#include "TxtSaveService.hpp"

TxtSaveService::TxtSaveService (const GameCycleService::GameState& state, const std::string& directory) :
    gameState(state), savesDirectory(directory) {}

void TxtSaveService::save (const std::string& saveName) {
    try {
        validateSaveName(saveName);
        ensureSavesDirectory();
        
        std::string filePath = getSavePath(saveName);
        std::ofstream file(filePath);
        
        if (!file.is_open()) {
            throw SaveLoadException("Cannot open file for writing: " + filePath);
        }
        
        file << "GAME_SAVE_FILE\n";
        
        file << "GAME_STATE\n";
        saveGameState(file);
        
        file << "FIELD_DATA\n";
        saveFieldData(file);
        
        file << "ENTITIES_DATA\n";
        saveEntitiesData(file);
        
        file << "SPELL_DATA\n";
        saveSpellData(file);
        
        file << "SPELL_STORE_DATA\n";
        saveSpellStoreData(file);
        
        file << "PLAYER_DATA\n";
        savePlayerData(file);
        
        file << "END_SAVE_FILE\n";
        
        if (file.fail()) {
            throw SaveLoadException("Write operation failed for save: " + saveName);
        }
        
    }
    catch (const std::filesystem::filesystem_error& e) {
        throw SaveLoadException("Filesystem error: " + std::string(e.what()));
    }
    catch (const std::ofstream::failure& e) {
        throw SaveLoadException("File I/O error: " + std::string(e.what()));
    }
}

void TxtSaveService::ensureSavesDirectory () const {
    std::filesystem::create_directories(savesDirectory);
}

std::string TxtSaveService::getSavePath (const std::string& saveName) const {
    return "../" + savesDirectory + "/" + saveName + ".txt";
}

void TxtSaveService::validateSaveName (const std::string& saveName) const {
    if (saveName.empty()) {
        throw SaveLoadException("Save name cannot be empty");
    }
    
    if (saveName.find('/') != std::string::npos || 
        saveName.find('\\') != std::string::npos ||
        saveName.find(':') != std::string::npos) {
        throw SaveLoadException("Save name contains invalid characters");
    }
}

void TxtSaveService::saveGameState (std::ofstream& file) const {
    GameStateSerializer stateSerializer(gameState);
    file << stateSerializer.serialize();
}

void TxtSaveService::saveFieldData (std::ofstream& file) const {
    FieldSerializer fieldSerializer(gameState.field);
    file << fieldSerializer.serialize();
}

void TxtSaveService::saveEntitiesData (std::ofstream& file) const {
    EntityManagerSerializer managerSerializer(gameState.manager);
    file << managerSerializer.serialize();
}

void TxtSaveService::saveSpellData (std::ofstream& file) const {
    SpellHandSerializer handSerializer(gameState.hand);
    file << handSerializer.serialize() << "\n";
}

void TxtSaveService::saveSpellStoreData (std::ofstream& file) const {
    SpellStoreSerializer storeSerializer(gameState.store);
    file << storeSerializer.serialize() << "\n";
}

void TxtSaveService::savePlayerData (std::ofstream& file) const {
    file << "player_data_complete\n";
}