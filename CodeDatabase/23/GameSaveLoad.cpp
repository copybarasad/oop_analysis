#include "GameSaveLoad.h"
#include "Constants.h"
#include "json.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <limits>

using json = nlohmann::json;

const std::string GameSaveLoad::SAVE_FILE_EXTENSION = ".json";
const std::string GameSaveLoad::DEFAULT_SAVE_FILE = "game_save" + SAVE_FILE_EXTENSION;

void GameSaveLoad::saveGame(const GameState& state, const std::string& filename) {
    if (saveFileExists(filename)) {
        std::cout << "Warning: Overwriting existing save file: " << filename << std::endl;
    } else {
        std::cout << "Creating new save file: " << filename << std::endl;
    }
    
    validateGameState(state, filename);
    
    json j;
    j["version"] = "1.0";
    j["currentLevel"] = state.currentLevel;
    j["player"]["hp"] = state.playerHp;
    j["player"]["baseDamage"] = state.playerBaseDmg;
    j["player"]["handCapacity"] = state.playerHandCapacity;
    j["player"]["score"] = state.playerScore;
    j["player"]["spells"] = json::array();
    for (const auto& spellName : state.playerSpells) {
        j["player"]["spells"].push_back(spellName);
    }
    j["field"]["width"] = state.fieldWidth;
    j["field"]["height"] = state.fieldHeight;
    
    {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "\n!!! ERROR: Cannot open file for writing: " << filename << " !!!" << std::endl;
            throw SaveException(filename, "Cannot open file for writing");
        }
        
        try {
            std::string jsonString = j.dump(4);
            file << jsonString;
            
            if (file.fail()) {
                std::cout << "\n!!! ERROR: Write operation failed for file: " << filename << " !!!" << std::endl;
                throw SaveException(filename, "Write operation failed");
            }
            
            file.flush();
            if (file.fail()) {
                std::cout << "\n!!! ERROR: Failed to flush data to disk for file: " << filename << " !!!" << std::endl;
                throw SaveException(filename, "Failed to flush data to disk");
            }
            
            if (file.bad()) {
                std::cout << "\n!!! ERROR: File stream is in bad state after write: " << filename << " !!!" << std::endl;
                throw SaveException(filename, "File stream is in bad state after write");
            }
            
        } catch (const SaveException&) {
            file.close();
            std::remove(filename.c_str());
            std::cout << "\n!!! ERROR: Save operation failed, corrupted file removed: " << filename << " !!!" << std::endl;
            throw;
        } catch (const json::exception& e) {
            file.close();
            std::remove(filename.c_str());
            std::cout << "\n!!! ERROR: JSON error during save: " << e.what() << " !!!" << std::endl;
            throw SaveException(filename, std::string("JSON error: ") + e.what());
        } catch (...) {
            file.close();
            std::remove(filename.c_str());
            std::cout << "\n!!! ERROR: Unknown error during save operation !!!" << std::endl;
            throw SaveException(filename, "Unknown error during save");
        }
    }
    
    if (!saveFileExists(filename)) {
        std::cout << "\n!!! ERROR: File was not created or is not readable after save: " << filename << " !!!" << std::endl;
        throw SaveException(filename, "File was not created or is not readable after save");
    }
    
    std::cout << "Save file written successfully: " << filename << std::endl;
}

GameState GameSaveLoad::loadGame(const std::string& filename) {
    if (!saveFileExists(filename)) {
        std::cout << "\n!!! ERROR: Save file not found: " << filename << " !!!" << std::endl;
        throw FileNotFoundException(filename);
    }
    
    std::cout << "Loading game from: " << filename << std::endl;
    
    json j;
    
    {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "\n!!! ERROR: Cannot open file for reading: " << filename << " !!!" << std::endl;
            throw LoadException(filename, "Cannot open file for reading");
        }
        
        try {
            file >> j;
            if (file.fail() && !file.eof()) {
                std::cout << "\n!!! ERROR: Read error or corrupted data in file: " << filename << " !!!" << std::endl;
                throw InvalidSaveFileException(filename, "Read error or corrupted data");
            }
        } catch (const json::parse_error& e) {
            std::cout << "\n!!! ERROR: JSON parse error in file " << filename << ": " << e.what() << " !!!" << std::endl;
            throw InvalidSaveFileException(filename, std::string("JSON parse error: ") + e.what());
        } catch (const json::exception& e) {
            std::cout << "\n!!! ERROR: JSON error in file " << filename << ": " << e.what() << " !!!" << std::endl;
            throw InvalidSaveFileException(filename, std::string("JSON error: ") + e.what());
        } catch (const LoadException&) {
            throw;
        } catch (...) {
            std::cout << "\n!!! ERROR: Unknown error during load from file: " << filename << " !!!" << std::endl;
            throw InvalidSaveFileException(filename, "Unknown error during load");
        }
    }
    
    GameState state;
    
    try {
        if (!j.contains("version")) {
            std::cout << "\n!!! ERROR: Missing version field in save file: " << filename << " !!!" << std::endl;
            throw InvalidSaveFileException(filename, "Missing version field");
        }
        
        if (!j.contains("currentLevel") || !j["currentLevel"].is_number_integer()) {
            std::cout << "\n!!! ERROR: Invalid or missing currentLevel in save file: " << filename << " !!!" << std::endl;
            throw InvalidSaveFileException(filename, "Invalid or missing currentLevel");
        }
        state.currentLevel = j["currentLevel"];
        
        if (!j.contains("player") || !j["player"].is_object()) {
            std::cout << "\n!!! ERROR: Invalid or missing player data in save file: " << filename << " !!!" << std::endl;
            throw InvalidSaveFileException(filename, "Invalid or missing player data");
        }
        const auto& player = j["player"];
        
        if (!player.contains("hp") || !player["hp"].is_number_integer()) {
            std::cout << "\n!!! ERROR: Invalid or missing player HP in save file: " << filename << " !!!" << std::endl;
            throw InvalidSaveFileException(filename, "Invalid or missing player HP");
        }
        state.playerHp = player["hp"];
        
        if (!player.contains("baseDamage") || !player["baseDamage"].is_number_integer()) {
            std::cout << "\n!!! ERROR: Invalid or missing player baseDamage in save file: " << filename << " !!!" << std::endl;
            throw InvalidSaveFileException(filename, "Invalid or missing player baseDamage");
        }
        state.playerBaseDmg = player["baseDamage"];
        
        if (!player.contains("handCapacity") || !player["handCapacity"].is_number_integer()) {
            std::cout << "\n!!! ERROR: Invalid or missing player handCapacity in save file: " << filename << " !!!" << std::endl;
            throw InvalidSaveFileException(filename, "Invalid or missing player handCapacity");
        }
        state.playerHandCapacity = player["handCapacity"];
        
        if (!player.contains("score") || !player["score"].is_number_integer()) {
            std::cout << "\n!!! ERROR: Invalid or missing player score in save file: " << filename << " !!!" << std::endl;
            throw InvalidSaveFileException(filename, "Invalid or missing player score");
        }
        state.playerScore = player["score"];
        
        if (player.contains("spells") && player["spells"].is_array()) {
            state.playerSpells.clear();
            for (const auto& spell : player["spells"]) {
                if (spell.is_string()) {
                    state.playerSpells.push_back(spell);
                }
            }
        }
        
        if (!j.contains("field") || !j["field"].is_object()) {
            std::cout << "\n!!! ERROR: Invalid or missing field data in save file: " << filename << " !!!" << std::endl;
            throw InvalidSaveFileException(filename, "Invalid or missing field data");
        }
        const auto& field = j["field"];
        
        if (!field.contains("width") || !field["width"].is_number_integer()) {
            std::cout << "\n!!! ERROR: Invalid or missing field width in save file: " << filename << " !!!" << std::endl;
            throw InvalidSaveFileException(filename, "Invalid or missing field width");
        }
        state.fieldWidth = field["width"];
        
        if (!field.contains("height") || !field["height"].is_number_integer()) {
            std::cout << "\n!!! ERROR: Invalid or missing field height in save file: " << filename << " !!!" << std::endl;
            throw InvalidSaveFileException(filename, "Invalid or missing field height");
        }
        state.fieldHeight = field["height"];
        
        validateGameState(state, filename);
        
    } catch (const InvalidSaveFileException&) {
        throw;
    } catch (const json::exception& e) {
        std::cout << "\n!!! ERROR: JSON parsing error in save file " << filename << ": " << e.what() << " !!!" << std::endl;
        throw InvalidSaveFileException(filename, std::string("JSON parsing error: ") + e.what());
    } catch (...) {
        std::cout << "\n!!! ERROR: Unknown error parsing save file: " << filename << " !!!" << std::endl;
        throw InvalidSaveFileException(filename, "Unknown error parsing save file");
    }
    
    std::cout << "Game loaded successfully from: " << filename << std::endl;
    return state;
}

bool GameSaveLoad::saveFileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

void GameSaveLoad::validateGameState(const GameState& state, const std::string& filename) {
    if (state.currentLevel < 1) {
        std::cout << "\n!!! ERROR: Invalid level value in save file " << filename << ": " << state.currentLevel << " !!!" << std::endl;
        throw InvalidSaveFileException(filename, "Invalid level value: " + std::to_string(state.currentLevel));
    }
    if (state.playerHp < 0) {
        std::cout << "\n!!! ERROR: Invalid player HP value in save file " << filename << ": " << state.playerHp << " !!!" << std::endl;
        throw InvalidSaveFileException(filename, "Invalid player HP value: " + std::to_string(state.playerHp));
    }
    if (state.playerBaseDmg < 0) {
        std::cout << "\n!!! ERROR: Invalid player base damage value in save file " << filename << ": " << state.playerBaseDmg << " !!!" << std::endl;
        throw InvalidSaveFileException(filename, "Invalid player base damage value: " + std::to_string(state.playerBaseDmg));
    }
    if (state.playerHandCapacity < 1) {
        std::cout << "\n!!! ERROR: Invalid hand capacity value in save file " << filename << ": " << state.playerHandCapacity << " !!!" << std::endl;
        throw InvalidSaveFileException(filename, "Invalid hand capacity value: " + std::to_string(state.playerHandCapacity));
    }
    if (state.fieldWidth < MIN_FIELD_SIZE || state.fieldWidth > MAX_FIELD_SIZE) {
        std::cout << "\n!!! ERROR: Invalid field width in save file " << filename << ": " << state.fieldWidth << " !!!" << std::endl;
        throw InvalidSaveFileException(filename, "Invalid field width: " + std::to_string(state.fieldWidth));
    }
    if (state.fieldHeight < MIN_FIELD_SIZE || state.fieldHeight > MAX_FIELD_SIZE) {
        std::cout << "\n!!! ERROR: Invalid field height in save file " << filename << ": " << state.fieldHeight << " !!!" << std::endl;
        throw InvalidSaveFileException(filename, "Invalid field height: " + std::to_string(state.fieldHeight));
    }
}

