#include "TxtLoadService.hpp"

TxtLoadService::TxtLoadService (const std::string& directory) :
    savesDirectory(directory) {}

GameCycleService::GameState* TxtLoadService::load (const std::string& saveName) {
std::string filePath = getSavePath(saveName);
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw SaveLoadException("Cannot open save file: " + filePath);
    }

    validateSaveFile(file, saveName);

    Field* field = nullptr;
    SpellHand* hand = nullptr;
    EntityManager* manager = nullptr;
    SpellStore* store = nullptr;
    size_t currentLevel = 0;
    GameProgress progress = GameProgress::ON_GOING;
    
    try {
        std::string line;
        while (std::getline(file, line)) {
            if (line == "GAME_STATE") {
                parseGameState(file, currentLevel, progress);
            }
            else if (line == "FIELD_DATA") {
                field = parseFieldData(file);
            }
            else if (line == "ENTITIES_DATA") {
                manager = parseEntitiesData(file, field);
            }
            else if (line == "SPELL_DATA") {
                hand = parseSpellData(file);
            }
            else if (line == "SPELL_STORE_DATA") {
                store = parseSpellStoreData(file);
            }
            else if (line == "PLAYER_DATA") {
                parsePlayerData(file);
            }
            else if (line == "END_SAVE_FILE") {
                break;
            }
        }

        if (!field || !hand || !manager || !store) {
            throw ValidationException("Not all game components were loaded");
        }
        GameCycleService::GameState* state = new GameCycleService::GameState(
            currentLevel, *field, *hand, *manager, *store
        );
        state->progress = progress;
        
        return state;
    }
    catch (...) {
        delete field;
        delete hand;
        delete manager;
        delete store;
        throw;
    }
}

bool TxtLoadService::saveExists (const std::string& saveName) const {
    std::string filePath = getSavePath(saveName);
    return std::filesystem::exists(filePath);
}

std::string TxtLoadService::getSavePath (const std::string& saveName) const {
    return "../" + savesDirectory + "/" + saveName + ".txt";
}

void TxtLoadService::validateSaveFile (std::ifstream& file, const std::string& saveName) const {
    std::string header = readLine(file);
    if (header != "GAME_SAVE_FILE") {
        throw ValidationException("Invalid save file format for: " + saveName);
    }
}

std::string TxtLoadService::readLine (std::ifstream& file) const {
    std::string line;
    if (!std::getline(file, line)) {
        throw SaveLoadException("Unexpected end of file");
    }
    return line;
}

void TxtLoadService::parseGameState(std::ifstream& file, size_t& currentLevel, GameProgress& progress) {
    std::string line;
    while (std::getline(file, line) && !line.empty()) {
        auto parts = deserializer.split(line, ':');
        if (parts.size() != 2) {
            throw ValidationException("Invalid game state format: " + line);
        }
        
        const std::string& key = parts[0];
        const std::string& value = parts[1];
        
        if (key == "currentLevel") {
            currentLevel = deserializer.parseSizeT(value, "currentLevel");
        } 
        else if (key == "progress") {
            if (value == "PLAYER_DEAD") {
                progress = GameProgress::PLAYER_DEAD;
            } 
            else if (value == "LEVEL_COMPLETED") {
                progress = GameProgress::LEVEL_COMPLETED;
            } 
            else if (value == "ON_GOING") {
                progress = GameProgress::ON_GOING;
            } 
            else {
                throw ValidationException("Invalid game progress: " + value);
            }
        }
    }
}

Field* TxtLoadService::parseFieldData (std::ifstream& file) {
    std::string fieldData;
    std::string line;

    while (std::getline(file, line) && !line.empty()) {
        fieldData += line + "\n";
    }
    
    FieldDeserializer fieldDeserializer;
    return fieldDeserializer.deserialize(fieldData);
}

EntityManager* TxtLoadService::parseEntitiesData (std::ifstream& file, Field* field) {
    std::string entitiesData;
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (line == "SPELL_DATA") {
            file.seekg(-static_cast<int>(line.length() + 1), std::ios_base::cur);
            break;
        }
        entitiesData += line + "\n";
    }
    
    EntityManagerDeserializer managerDeserializer;
    return managerDeserializer.deserialize(entitiesData, field);
}

SpellHand* TxtLoadService::parseSpellData (std::ifstream& file) {
    std::string spellData = readLine(file);
    
    SpellHandDeserializer handDeserializer;
    return handDeserializer.deserialize(spellData);
}

SpellStore* TxtLoadService::parseSpellStoreData (std::ifstream& file) {
    std::string storeData = readLine(file);
    
    SpellStoreDeserializer storeDeserializer;
    return storeDeserializer.deserialize(storeData);
}

void TxtLoadService::parsePlayerData (std::ifstream& file) {
    std::string playerData = readLine(file);
    if (playerData != "player_data_complete") {
        throw ValidationException("Invalid player data section");
    }
}