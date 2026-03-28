#include "SaveManager.hpp"
#include "Configs.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <map>

// Создание директории для сохранений если она не существует
bool SaveManager::ensureSaveDirectoryExists() {
    std::string directory = getSaveDirectoryPath();
    try {
        return std::filesystem::create_directories(directory);
    } catch (const std::exception& e) {
        std::cerr << "Error creating save directory: " << e.what() << std::endl;
        return false;
    }
}

// Получение пути к директории сохранений
std::string SaveManager::getSaveDirectoryPath() {
    return Config::Settings::SAVE_DIRECTORY;
}

// Сохранение игры в файл
bool SaveManager::saveGame(const std::string& filename, const Field& field, int level) {
    try {
        // Проверяем существование директории
        ensureSaveDirectoryExists();
        
        std::string fullPath = getSaveDirectoryPath() + filename + Config::Settings::SAVE_EXTENSION;
        std::ofstream file(fullPath);
        
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file for writing: " << fullPath << std::endl;
            return false;
        }
        
        // Записываем заголовок
        file << "[GameSave]\n";
        
        // Сохраняем уровень
        file << "Level=" << level << "\n";
        
        // Сохраняем информацию о поле
        file << "FieldWidth=" << field.getWidth() << "\n";
        file << "FieldHeight=" << field.getHeight() << "\n";
        
        // Сохраняем информацию о игроке
        const Player& player = field.getPlayer();
        file << "PlayerHealth=" << player.getHealth() << "\n";
        file << "PlayerDamage=" << player.getDamage() << "\n";
        file << "PlayerScore=" << player.getScore() << "\n";
        file << "PlayerRangedMode=" << (player.getIsRangedMode() ? "1" : "0") << "\n";
        
        Pos playerPos = field.getPlayerPosition();
        file << "PlayerX=" << playerPos.getX() << "\n";
        file << "PlayerY=" << playerPos.getY() << "\n";
        
        // Сохраняем информацию о врагах
        auto enemies = field.getEnemiesWithPositions();
        file << "EnemyCount=" << enemies.size() << "\n";
        
        for (size_t i = 0; i < enemies.size(); ++i) {
            const auto& enemy = enemies[i].first;
            const auto& pos = enemies[i].second;
            file << "Enemy" << i << "_X=" << pos.getX() << "\n";
            file << "Enemy" << i << "_Y=" << pos.getY() << "\n";
            file << "Enemy" << i << "_Health=" << enemy.getHealth() << "\n";
            file << "Enemy" << i << "_Damage=" << enemy.getDamage() << "\n";
        }
        
        // Сохраняем информацию о сетке поля
        file << "Grid=";
        for (int y = 0; y < field.getHeight(); ++y) {
            for (int x = 0; x < field.getWidth(); ++x) {
                CellType cellType = field.getCellType(Pos(x, y));
                file << static_cast<int>(cellType);
                if (!(y == field.getHeight() - 1 && x == field.getWidth() - 1)) {
                    file << ",";
                }
            }
        }
        file << "\n";
        
        file.close();
        std::cout << "Game saved successfully: " << fullPath << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error saving game: " << e.what() << std::endl;
        return false;
    }
}

// Загрузка игры из файла
bool SaveManager::loadGame(const std::string& filename, Field& field, int& level) {
    try {
        std::string fullPath = getSaveDirectoryPath() + filename + Config::Settings::SAVE_EXTENSION;
        std::ifstream file(fullPath);
        
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open save file: " << fullPath << std::endl;
            return false;
        }
        
        std::string line;
        std::map<std::string, std::string> data;
        
        // Читаем файл и парсим данные
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '[') continue;
            
            size_t delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos) {
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);
                data[key] = value;
            }
        }
        
        file.close();
        
        // Проверяем наличие обязательных данных
        if (data.find("Level") == data.end() || data.find("FieldWidth") == data.end()) {
            std::cerr << "Error: Corrupted save file - missing required data" << std::endl;
            return false;
        }
        
        // Загружаем уровень
        level = std::stoi(data["Level"]);
        
        // Создаем новое поле с загруженными размерами
        int width = std::stoi(data["FieldWidth"]);
        int height = std::stoi(data["FieldHeight"]);
        Field newField;
        newField.resizeGrid(width, height);
        
        // Восстанавливаем игрока
        Player& player = newField.getPlayer();
        player.setHealth(std::stoi(data["PlayerHealth"]));
        player.setScore(std::stoi(data["PlayerScore"]));
        player.setRangedMode(std::stoi(data["PlayerRangedMode"]) != 0);
        
        // Восстанавливаем позицию игрока
        int playerX = std::stoi(data["PlayerX"]);
        int playerY = std::stoi(data["PlayerY"]);
        newField.setPlayerPosition(Pos(playerX, playerY));
        
        // Очищаем существующих врагов и добавляем загруженных
        newField.clearEnemies();
        int enemyCount = std::stoi(data["EnemyCount"]);
        for (int i = 0; i < enemyCount; ++i) {
            std::string prefix = "Enemy" + std::to_string(i);
            int x = std::stoi(data[prefix + "_X"]);
            int y = std::stoi(data[prefix + "_Y"]);
            int health = std::stoi(data[prefix + "_Health"]);
            int damage = std::stoi(data[prefix + "_Damage"]);
            
            auto enemy = std::make_unique<Enemy>(health, damage);
            newField.addEnemy(std::move(enemy), Pos(x, y));
        }
        
        // Восстанавливаем сетку поля
        std::string gridData = data["Grid"];
        std::stringstream ss(gridData);
        std::string cell;
        
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (!std::getline(ss, cell, ',')) {
                    break;
                }
                int cellValue = std::stoi(cell);
                newField.setCellType(Pos(x, y), static_cast<CellType>(cellValue));
            }
        }
        
        field = std::move(newField);
        std::cout << "Game loaded successfully: " << fullPath << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error loading game: " << e.what() << std::endl;
        return false;
    }
}

// Получение списка сохранений
std::vector<SaveInfo> SaveManager::getSaveFiles() {
    std::vector<SaveInfo> saves;
    
    try {
        std::string directory = getSaveDirectoryPath();
        if (!std::filesystem::exists(directory)) {
            return saves;
        }
        
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_regular_file() && entry.path().extension() == Config::Settings::SAVE_EXTENSION) {
                SaveInfo info;
                info.filename = entry.path().stem().string();
                
                // Парсим базовую информацию из файла для отображения
                try {
                    std::ifstream file(entry.path());
                    std::string line;
                    while (std::getline(file, line)) {
                        if (line.find("Level=") != std::string::npos) {
                            info.level = std::stoi(line.substr(6));
                        } else if (line.find("PlayerScore=") != std::string::npos) {
                            info.playerScore = std::stoi(line.substr(12));
                        }
                    }
                    file.close();
                } catch (...) {
                    info.level = 1;
                    info.playerScore = 0;
                }
                
                // Время создания файла
                auto ftime = std::filesystem::last_write_time(entry.path());
                auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                    ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
                std::time_t tt = std::chrono::system_clock::to_time_t(sctp);
                std::tm* tm = std::localtime(&tt);
                
                std::ostringstream oss;
                oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
                info.timestamp = oss.str();
                
                saves.push_back(info);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error reading save files: " << e.what() << std::endl;
    }
    
    return saves;
}

// Генерация имени для сохранения
std::string SaveManager::generateSaveName() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "save_%Y%m%d_%H%M%S");
    return oss.str();
}
