#include "GameState.h"
#include <fstream>
#include <iostream>

using namespace std;

GameState::GameState() : currentLevel(1), fieldSize(0), killCount(0) {
    playerData.health = 0;
    playerData.damage = 0;
    playerData.score = 0;
    playerData.fightingStyle = 'm';
    playerData.attackRange = 0;
    playerData.skipNextMove = false;
    playerData.buffCount = 0;
}

void GameState::saveToFile(const std::string& filename) const {
    ofstream file(filename, ios::binary);
    if (!file) {
        throw SaveGameException("Cannot open file for writing: " + filename);
    }
    
    try {
        serializeData(file);
        
        if (!file.good()) {
            throw SaveGameException("Error during file writing: " + filename);
        }
    }
    catch (const exception& e) {
        throw SaveGameException(string("Serialization error: ") + e.what());
    }
    file.close();
}

void GameState::loadFromFile(const std::string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        throw LoadGameException("Cannot open file for reading: " + filename);
    }
    
    try {
        deserializeData(file);
        
        if (!file.good() && !file.eof()) {
            throw LoadGameException("Error during file reading: " + filename);
        }
    }
    catch (const exception& e) {
        throw LoadGameException(string("Deserialization error: ") + e.what());
    }
    file.close();
}

void GameState::serializeData(std::ofstream& file) const {
    // Сохраняем базовые параметры
    file.write(reinterpret_cast<const char*>(&currentLevel), sizeof(currentLevel));
    file.write(reinterpret_cast<const char*>(&fieldSize), sizeof(fieldSize));
    file.write(reinterpret_cast<const char*>(&killCount), sizeof(killCount));
    
    // Сохраняем данные игрока
    file.write(reinterpret_cast<const char*>(&playerData.health), sizeof(playerData.health));
    file.write(reinterpret_cast<const char*>(&playerData.damage), sizeof(playerData.damage));
    file.write(reinterpret_cast<const char*>(&playerData.position), sizeof(playerData.position));
    file.write(reinterpret_cast<const char*>(&playerData.score), sizeof(playerData.score));
    file.write(reinterpret_cast<const char*>(&playerData.fightingStyle), sizeof(playerData.fightingStyle));
    file.write(reinterpret_cast<const char*>(&playerData.attackRange), sizeof(playerData.attackRange));
    file.write(reinterpret_cast<const char*>(&playerData.skipNextMove), sizeof(playerData.skipNextMove));
    file.write(reinterpret_cast<const char*>(&playerData.damageBonus), sizeof(playerData.damageBonus));
    file.write(reinterpret_cast<const char*>(&playerData.buffCount), sizeof(playerData.buffCount)); // ДОБАВИТЬ buffCount
    
    // Сохраняем заклинания игрока
    size_t spellCount = playerData.spellNames.size();
    file.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));
    for (const auto& spellName : playerData.spellNames) {
        size_t nameLength = spellName.length();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(spellName.c_str(), nameLength);
    }
    
    // Сохраняем врагов
    size_t enemyCount = enemiesData.size();
    file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
    for (const auto& enemy : enemiesData) {
        file.write(reinterpret_cast<const char*>(&enemy), sizeof(enemy));
    }
    
    // Сохраняем здания
    size_t buildingCount = buildingsData.size();
    file.write(reinterpret_cast<const char*>(&buildingCount), sizeof(buildingCount));
    for (const auto& building : buildingsData) {
        file.write(reinterpret_cast<const char*>(&building), sizeof(building));
    }
    
    // Сохраняем союзников
    size_t allyCount = alliesData.size();
    file.write(reinterpret_cast<const char*>(&allyCount), sizeof(allyCount));
    for (const auto& ally : alliesData) {
        file.write(reinterpret_cast<const char*>(&ally), sizeof(ally));
    }
    
    // Сохраняем поле
    size_t fieldCellCount = fieldData.size();
    file.write(reinterpret_cast<const char*>(&fieldCellCount), sizeof(fieldCellCount));
    for (const auto& cell : fieldData) {
        file.write(reinterpret_cast<const char*>(&cell), sizeof(cell));
    }
}

void GameState::deserializeData(std::ifstream& file) {
    // Загружаем базовые параметры
    file.read(reinterpret_cast<char*>(&currentLevel), sizeof(currentLevel));
    file.read(reinterpret_cast<char*>(&fieldSize), sizeof(fieldSize));
    file.read(reinterpret_cast<char*>(&killCount), sizeof(killCount));
    
    // Загружаем данные игрока
    file.read(reinterpret_cast<char*>(&playerData.health), sizeof(playerData.health));
    file.read(reinterpret_cast<char*>(&playerData.damage), sizeof(playerData.damage));
    file.read(reinterpret_cast<char*>(&playerData.position), sizeof(playerData.position));
    file.read(reinterpret_cast<char*>(&playerData.score), sizeof(playerData.score));
    file.read(reinterpret_cast<char*>(&playerData.fightingStyle), sizeof(playerData.fightingStyle));
    file.read(reinterpret_cast<char*>(&playerData.attackRange), sizeof(playerData.attackRange));
    file.read(reinterpret_cast<char*>(&playerData.skipNextMove), sizeof(playerData.skipNextMove));
    file.read(reinterpret_cast<char*>(&playerData.damageBonus), sizeof(playerData.damageBonus));
    file.read(reinterpret_cast<char*>(&playerData.buffCount), sizeof(playerData.buffCount));
    
    // Загружаем заклинания игрока
    size_t spellCount;
    file.read(reinterpret_cast<char*>(&spellCount), sizeof(spellCount));
    playerData.spellNames.clear();
    for (size_t i = 0; i < spellCount; i++) {
        size_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        std::string spellName(nameLength, ' ');
        file.read(&spellName[0], nameLength);
        playerData.spellNames.push_back(spellName);
    }
    
    // Загружаем врагов
    size_t enemyCount;
    file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
    enemiesData.resize(enemyCount);
    for (size_t i = 0; i < enemyCount; i++) {
        file.read(reinterpret_cast<char*>(&enemiesData[i]), sizeof(UnitData));
    }
    
    // Загружаем здания
    size_t buildingCount;
    file.read(reinterpret_cast<char*>(&buildingCount), sizeof(buildingCount));
    buildingsData.resize(buildingCount);
    for (size_t i = 0; i < buildingCount; i++) {
        file.read(reinterpret_cast<char*>(&buildingsData[i]), sizeof(UnitData));
    }
    
    // Загружаем союзников
    size_t allyCount;
    file.read(reinterpret_cast<char*>(&allyCount), sizeof(allyCount));
    alliesData.resize(allyCount);
    for (size_t i = 0; i < allyCount; i++) {
        file.read(reinterpret_cast<char*>(&alliesData[i]), sizeof(UnitData));
    }
    
    // Загружаем поле
    size_t fieldCellCount;
    file.read(reinterpret_cast<char*>(&fieldCellCount), sizeof(fieldCellCount));
    fieldData.resize(fieldCellCount);
    for (size_t i = 0; i < fieldCellCount; i++) {
        file.read(reinterpret_cast<char*>(&fieldData[i]), sizeof(FieldCellData));
    }
}