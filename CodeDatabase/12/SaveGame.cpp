#include "SaveGame.h"
#include <iostream>
#include <sstream>
#include <filesystem>
#include <fstream>

const std::string SaveSystem::SAVE_FILENAME = "game_save.dat";
const std::string SaveSystem::SAVE_FILE_VERSION = "GAME_SAVE_V2";

void SaveSystem::validateSaveData(const SaveGame& data) {
    if (data.playerHealth < 0) {
        throw SaveLoadException("Некорректное здоровье игрока: " + std::to_string(data.playerHealth));
    }
    if (data.playerX < 0 || data.playerY < 0) {
        throw SaveLoadException("Некорректные координаты игрока: (" + 
                               std::to_string(data.playerX) + ", " + 
                               std::to_string(data.playerY) + ")");
    }
    
    if (data.fieldWidth <= 0 || data.fieldHeight <= 0) {
        throw SaveLoadException("Некорректные размеры поля: " + 
                               std::to_string(data.fieldWidth) + "x" + 
                               std::to_string(data.fieldHeight));
    }
    
    if (data.playerX >= data.fieldWidth || data.playerY >= data.fieldHeight) {
        throw SaveLoadException("Игрок находится за пределами поля: (" + 
                               std::to_string(data.playerX) + ", " + 
                               std::to_string(data.playerY) + ") при размере " + 
                               std::to_string(data.fieldWidth) + "x" + 
                               std::to_string(data.fieldHeight));
    }
    
    if (data.currentLevel < 1 || data.currentLevel > 5) {
        throw SaveLoadException("Некорректный уровень: " + std::to_string(data.currentLevel));
    }
    
    for (size_t i = 0; i < data.enemies.size(); ++i) {
        const auto& enemy = data.enemies[i];
        if (enemy.health < 0) {
            throw SaveLoadException("Некорректное здоровье врага #" + std::to_string(i) + 
                                   ": " + std::to_string(enemy.health));
        }
        if (enemy.x < 0 || enemy.y < 0 || enemy.x >= data.fieldWidth || enemy.y >= data.fieldHeight) {
            throw SaveLoadException("Враг #" + std::to_string(i) + " за пределами поля: (" + 
                                   std::to_string(enemy.x) + ", " + std::to_string(enemy.y) + ")");
        }
    }
    
    if (data.turnCount < 0) {
        throw SaveLoadException("Некорректный счетчик ходов: " + std::to_string(data.turnCount));
    }
}

void SaveSystem::validateFileHeader(std::ifstream& file) {
    std::string version;
    if (!std::getline(file, version)) {
        throw SaveLoadException("Не удалось прочитать версию файла сохранения");
    }
    
    if (version != SAVE_FILE_VERSION) {
        throw SaveLoadException("Несовместимая версия файла сохранения. Ожидается: " + 
                               SAVE_FILE_VERSION + ", получено: " + version);
    }
}

bool SaveSystem::saveGame(const SaveGame& data) {
    try {
        validateSaveData(data);
        
        std::filesystem::path savePath(SAVE_FILENAME);
        auto parentPath = savePath.parent_path();
        if (!parentPath.empty() && !std::filesystem::exists(parentPath)) {
            if (!std::filesystem::create_directories(parentPath)) {
                throw SaveLoadException("Не удалось создать директорию для сохранения: " + parentPath.string());
            }
        }
        
        std::ofstream testFile(SAVE_FILENAME, std::ios::app);
        if (!testFile.is_open()) {
            throw SaveLoadException("Нет прав на запись в файл: " + SAVE_FILENAME);
        }
        testFile.close();
        
        std::ofstream file(SAVE_FILENAME, std::ios::binary);
        if (!file.is_open()) {
            throw SaveLoadException("Не удалось создать файл сохранения: " + SAVE_FILENAME);
        }
        
        file << SAVE_FILE_VERSION << "\n";
        
        file.write(reinterpret_cast<const char*>(&data.playerX), sizeof(data.playerX));
        file.write(reinterpret_cast<const char*>(&data.playerY), sizeof(data.playerY));
        file.write(reinterpret_cast<const char*>(&data.playerHealth), sizeof(data.playerHealth));
        file.write(reinterpret_cast<const char*>(&data.playerAlive), sizeof(data.playerAlive));
        file.write(reinterpret_cast<const char*>(&data.playerMeleeDamage), sizeof(data.playerMeleeDamage));
        file.write(reinterpret_cast<const char*>(&data.playerRangedDamage), sizeof(data.playerRangedDamage));
        file.write(reinterpret_cast<const char*>(&data.playerIsMelee), sizeof(data.playerIsMelee));
        
        if (file.fail()) {
            throw SaveLoadException("Ошибка записи данных игрока");
        }
        
        file.write(reinterpret_cast<const char*>(&data.fieldWidth), sizeof(data.fieldWidth));
        file.write(reinterpret_cast<const char*>(&data.fieldHeight), sizeof(data.fieldHeight));
        file.write(reinterpret_cast<const char*>(&data.hasActiveTrap), sizeof(data.hasActiveTrap));
        file.write(reinterpret_cast<const char*>(&data.trapX), sizeof(data.trapX));
        file.write(reinterpret_cast<const char*>(&data.trapY), sizeof(data.trapY));
        file.write(reinterpret_cast<const char*>(&data.trapDamage), sizeof(data.trapDamage));
        
        if (file.fail()) {
            throw SaveLoadException("Ошибка записи данных поля");
        }
        
        file.write(reinterpret_cast<const char*>(&data.currentLevel), sizeof(data.currentLevel));
        
        size_t enemyCount = data.enemies.size();
        file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
        
        for (const auto& enemy : data.enemies) {
            file.write(reinterpret_cast<const char*>(&enemy.x), sizeof(enemy.x));
            file.write(reinterpret_cast<const char*>(&enemy.y), sizeof(enemy.y));
            file.write(reinterpret_cast<const char*>(&enemy.health), sizeof(enemy.health));
            file.write(reinterpret_cast<const char*>(&enemy.damage), sizeof(enemy.damage));
            file.write(reinterpret_cast<const char*>(&enemy.alive), sizeof(enemy.alive));
        }
        
        if (file.fail()) {
            throw SaveLoadException("Ошибка записи данных врагов");
        }
        
        size_t towerCount = data.towers.size();
        file.write(reinterpret_cast<const char*>(&towerCount), sizeof(towerCount));
        
        for (const auto& tower : data.towers) {
            file.write(reinterpret_cast<const char*>(&tower.x), sizeof(tower.x));
            file.write(reinterpret_cast<const char*>(&tower.y), sizeof(tower.y));
            file.write(reinterpret_cast<const char*>(&tower.range), sizeof(tower.range));
            file.write(reinterpret_cast<const char*>(&tower.health), sizeof(tower.health));
            file.write(reinterpret_cast<const char*>(&tower.cooldown), sizeof(tower.cooldown));
            file.write(reinterpret_cast<const char*>(&tower.currentCooldown), sizeof(tower.currentCooldown));
            file.write(reinterpret_cast<const char*>(&tower.alive), sizeof(tower.alive));
        }
        
        if (file.fail()) {
            throw SaveLoadException("Ошибка записи данных башен");
        }
        
        size_t spellCount = data.spells.size();
        file.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));
        
        for (const auto& spell : data.spells) {
            size_t typeLength = spell.type.size();
            file.write(reinterpret_cast<const char*>(&typeLength), sizeof(typeLength));
            file.write(spell.type.c_str(), typeLength);
            file.write(reinterpret_cast<const char*>(&spell.param1), sizeof(spell.param1));
            file.write(reinterpret_cast<const char*>(&spell.param2), sizeof(spell.param2));
        }
        
        if (file.fail()) {
            throw SaveLoadException("Ошибка записи данных заклинаний");
        }
        
        size_t wallCount = data.walls.size();
        file.write(reinterpret_cast<const char*>(&wallCount), sizeof(wallCount));
        
        for (const auto& wall : data.walls) {
            file.write(reinterpret_cast<const char*>(&wall.x), sizeof(wall.x));
            file.write(reinterpret_cast<const char*>(&wall.y), sizeof(wall.y));
        }
        
        if (file.fail()) {
            throw SaveLoadException("Ошибка записи данных стен");
        }
        
        file.write(reinterpret_cast<const char*>(&data.turnCount), sizeof(data.turnCount));
        
        if (file.fail()) {
            throw SaveLoadException("Ошибка записи общих данных");
        }
        
        file.close();
        
        std::filesystem::path p(SAVE_FILENAME);
        if (std::filesystem::file_size(p) == 0) {
            throw SaveLoadException("Файл сохранения пуст");
        }
        
        std::cout << "Игра успешно сохранена! Уровень: " << data.currentLevel << std::endl;
        return true;
        
    } catch (const SaveLoadException& e) {
        std::cerr << "Ошибка сохранения: " << e.what() << std::endl;
        try {
            std::remove(SAVE_FILENAME.c_str());
        } catch (...) {
        }
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Неожиданная ошибка при сохранении: " << e.what() << std::endl;
        try {
            std::remove(SAVE_FILENAME.c_str());
        } catch (...) {
        }
        return false;
    }
}

bool SaveSystem::loadGame(SaveGame& data) {
    try {
        if (!saveExists()) {
            throw SaveLoadException("Файл сохранения не существует: " + SAVE_FILENAME);
        }
        
        std::filesystem::path p(SAVE_FILENAME);
        auto fileSize = std::filesystem::file_size(p);
        if (fileSize == 0) {
            throw SaveLoadException("Файл сохранения пуст");
        }
        if (fileSize > 10 * 1024 * 1024) { 
            throw SaveLoadException("Файл сохранения слишком большой: " + std::to_string(fileSize) + " байт");
        }
        
        std::ifstream file(SAVE_FILENAME, std::ios::binary);
        if (!file.is_open()) {
            throw SaveLoadException("Не удалось открыть файл сохранения: " + SAVE_FILENAME);
        }
        
        validateFileHeader(file);
        
        file.read(reinterpret_cast<char*>(&data.playerX), sizeof(data.playerX));
        file.read(reinterpret_cast<char*>(&data.playerY), sizeof(data.playerY));
        file.read(reinterpret_cast<char*>(&data.playerHealth), sizeof(data.playerHealth));
        file.read(reinterpret_cast<char*>(&data.playerAlive), sizeof(data.playerAlive));
        file.read(reinterpret_cast<char*>(&data.playerMeleeDamage), sizeof(data.playerMeleeDamage));
        file.read(reinterpret_cast<char*>(&data.playerRangedDamage), sizeof(data.playerRangedDamage));
        file.read(reinterpret_cast<char*>(&data.playerIsMelee), sizeof(data.playerIsMelee));
        
        if (file.fail() || file.eof()) {
            throw SaveLoadException("Ошибка чтения данных игрока или преждевременный конец файла");
        }
        
        file.read(reinterpret_cast<char*>(&data.fieldWidth), sizeof(data.fieldWidth));
        file.read(reinterpret_cast<char*>(&data.fieldHeight), sizeof(data.fieldHeight));
        file.read(reinterpret_cast<char*>(&data.hasActiveTrap), sizeof(data.hasActiveTrap));
        file.read(reinterpret_cast<char*>(&data.trapX), sizeof(data.trapX));
        file.read(reinterpret_cast<char*>(&data.trapY), sizeof(data.trapY));
        file.read(reinterpret_cast<char*>(&data.trapDamage), sizeof(data.trapDamage));
        
        if (file.fail() || file.eof()) {
            throw SaveLoadException("Ошибка чтения данных поля или преждевременный конец файла");
        }
        
        file.read(reinterpret_cast<char*>(&data.currentLevel), sizeof(data.currentLevel));
        
        size_t enemyCount;
        file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
        
        if (enemyCount > 1000) {
            throw SaveLoadException("Некорректное количество врагов: " + std::to_string(enemyCount));
        }
        
        data.enemies.resize(enemyCount);
        for (size_t i = 0; i < enemyCount; ++i) {
            file.read(reinterpret_cast<char*>(&data.enemies[i].x), sizeof(data.enemies[i].x));
            file.read(reinterpret_cast<char*>(&data.enemies[i].y), sizeof(data.enemies[i].y));
            file.read(reinterpret_cast<char*>(&data.enemies[i].health), sizeof(data.enemies[i].health));
            file.read(reinterpret_cast<char*>(&data.enemies[i].damage), sizeof(data.enemies[i].damage));
            file.read(reinterpret_cast<char*>(&data.enemies[i].alive), sizeof(data.enemies[i].alive));
            
            if (file.fail() || file.eof()) {
                throw SaveLoadException("Ошибка чтения данных врага #" + std::to_string(i));
            }
        }
        
        size_t towerCount;
        file.read(reinterpret_cast<char*>(&towerCount), sizeof(towerCount));
        
        if (towerCount > 100) {
            throw SaveLoadException("Некорректное количество башен: " + std::to_string(towerCount));
        }
        
        data.towers.resize(towerCount);
        for (size_t i = 0; i < towerCount; ++i) {
            file.read(reinterpret_cast<char*>(&data.towers[i].x), sizeof(data.towers[i].x));
            file.read(reinterpret_cast<char*>(&data.towers[i].y), sizeof(data.towers[i].y));
            file.read(reinterpret_cast<char*>(&data.towers[i].range), sizeof(data.towers[i].range));
            file.read(reinterpret_cast<char*>(&data.towers[i].health), sizeof(data.towers[i].health));
            file.read(reinterpret_cast<char*>(&data.towers[i].cooldown), sizeof(data.towers[i].cooldown));
            file.read(reinterpret_cast<char*>(&data.towers[i].currentCooldown), sizeof(data.towers[i].currentCooldown));
            file.read(reinterpret_cast<char*>(&data.towers[i].alive), sizeof(data.towers[i].alive));
            
            if (file.fail() || file.eof()) {
                throw SaveLoadException("Ошибка чтения данных башни #" + std::to_string(i));
            }
        }
        
        size_t spellCount;
        file.read(reinterpret_cast<char*>(&spellCount), sizeof(spellCount));
        
        if (spellCount > 100) {
            throw SaveLoadException("Некорректное количество заклинаний: " + std::to_string(spellCount));
        }
        
        data.spells.resize(spellCount);
        for (size_t i = 0; i < spellCount; ++i) {
            size_t typeLength;
            file.read(reinterpret_cast<char*>(&typeLength), sizeof(typeLength));
            
            if (typeLength > 100) { 
                throw SaveLoadException("Некорректная длина типа заклинания: " + std::to_string(typeLength));
            }
            
            data.spells[i].type.resize(typeLength);
            file.read(&data.spells[i].type[0], typeLength);
            file.read(reinterpret_cast<char*>(&data.spells[i].param1), sizeof(data.spells[i].param1));
            file.read(reinterpret_cast<char*>(&data.spells[i].param2), sizeof(data.spells[i].param2));
            
            if (file.fail() || file.eof()) {
                throw SaveLoadException("Ошибка чтения данных заклинания #" + std::to_string(i));
            }
        }
        
        size_t wallCount;
        file.read(reinterpret_cast<char*>(&wallCount), sizeof(wallCount));
        
        if (wallCount > data.fieldWidth * data.fieldHeight) {
            throw SaveLoadException("Некорректное количество стен: " + std::to_string(wallCount));
        }
        
        data.walls.resize(wallCount);
        for (size_t i = 0; i < wallCount; ++i) {
            file.read(reinterpret_cast<char*>(&data.walls[i].x), sizeof(data.walls[i].x));
            file.read(reinterpret_cast<char*>(&data.walls[i].y), sizeof(data.walls[i].y));
            
            if (file.fail() || file.eof()) {
                throw SaveLoadException("Ошибка чтения данных стены #" + std::to_string(i));
            }
        }
        
        file.read(reinterpret_cast<char*>(&data.turnCount), sizeof(data.turnCount));
        
        if (file.fail()) {
            throw SaveLoadException("Ошибка чтения общих данных");
        }

        file.close();
        
        validateSaveData(data);
        
        std::cout << "Игра успешно загружена! Уровень: " << data.currentLevel << std::endl;
        return true;
        
    } catch (const SaveLoadException& e) {
        std::cerr << "Ошибка загрузки: " << e.what() << std::endl;
        data = SaveGame{};
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Неожиданная ошибка при загрузке: " << e.what() << std::endl;
        data = SaveGame{};
        return false;
    }
}

bool SaveSystem::saveExists() {
    try {
        return std::filesystem::exists(SAVE_FILENAME) && 
               std::filesystem::file_size(SAVE_FILENAME) > 0;
    } catch (const std::exception&) {
        return false;
    }
}

void SaveSystem::showSaveMenu(SaveGame& data) {
    std::cout << "=== ЗАГРУЗКА СОХРАНЕНИЯ ===" << std::endl;
    std::cout << "Обнаружено сохранение игры." << std::endl;
    std::cout << "1. Загрузить сохраненную игру" << std::endl;
    std::cout << "2. Начать новую игру" << std::endl;
    std::cout << "Выберите действие: ";
    
    int choice;
    std::cin >> choice;
    
    if (choice == 1) {
        if (!loadGame(data)) {
            std::cout << "Не удалось загрузить сохранение. Начинаем новую игру." << std::endl;
        }
    } else {
        std::cout << "Начинаем новую игру." << std::endl;
    }
}

void SaveSystem::deleteSave() {
    try {
        if (std::filesystem::exists(SAVE_FILENAME)) {
            std::filesystem::remove(SAVE_FILENAME);
            std::cout << "Сохранение удалено." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при удалении сохранения: " << e.what() << std::endl;
    }
}