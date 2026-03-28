#include "JSONSaveManager.h"
#include <fstream>
#include <iostream>
#include <filesystem>

// Определение статических переменных
std::string JSONSaveManager::m_lastError;
const std::string JSONSaveManager::SECRET_KEY = "ANTI_CHIT_SAVE";

std::string JSONSaveManager::calculateChecksum(const std::string& data) {
    // Простая быстрая хеш-функция
    size_t hash = 5381;
    for (char c : data + SECRET_KEY) {
        hash = ((hash << 5) + hash) + c;
    }

    hash = hash ^ (hash >> 16);
    hash = hash * 0x85ebca6b;
    hash = hash ^ (hash >> 13);
    hash = hash * 0xc2b2ae35;
    hash = hash ^ (hash >> 16);

    return std::to_string(hash);
}

bool JSONSaveManager::handleSaveError(const std::string& error) {
    m_lastError = error;
    std::cerr << "SAVE ERROR: " << error << std::endl;
    return false;
}

bool JSONSaveManager::handleLoadError(const std::string& error) {
    m_lastError = error;
    std::cerr << "LOAD ERROR: " << error << std::endl;
    return false;
}

bool JSONSaveManager::validateJSONStructure(const nlohmann::json& j) {
    // Проверка обязательных полей
    std::vector<std::string> requiredFields = {
        "turnCount", "playerHealth", "level", "gameRunning",
        "playerX", "playerY", "fieldWidth", "fieldHeight", "grid", "checksum"
    };

    for (const auto& field : requiredFields) {
        if (!j.contains(field)) {
            m_lastError = "Missing required field in JSON: " + field;
            return false;
        }
    }

    // Проверка типов данных
    if (!j["turnCount"].is_number_integer() ||
        !j["playerHealth"].is_number_integer() ||
        !j["level"].is_number_integer() ||
        !j["playerX"].is_number_integer() ||
        !j["playerY"].is_number_integer() ||
        !j["checksum"].is_string()) {
        m_lastError = "Invalid data types in JSON";
        return false;
    }

    // Проверка структуры сетки
    if (!j["grid"].is_array()) {
        m_lastError = "Grid must be an array";
        return false;
    }

    return true;
}

bool JSONSaveManager::saveGameState(const GameState& gameState, const std::string& filename) {
    clearLastError();

    try {
        // Проверка валидности данных перед сохранением
        if (!SaveDataValidator::validateGameState(gameState)) {
            return handleSaveError("Game state validation failed before saving");
        }

        // Создание директорий если нужно
        auto path = std::filesystem::path(filename);
        if (path.has_parent_path()) {
            std::filesystem::create_directories(path.parent_path());
        }

        // Проверка возможности записи в директорию
        if (!std::filesystem::exists(path.parent_path())) {
            return handleSaveError("Cannot create directory for save file: " + path.parent_path().string());
        }

        // Конвертация в JSON
        nlohmann::json j = gameStateToJSON(gameState);

        // Запись в файл
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            return handleSaveError("Cannot open file for writing: " + filename);
        }

        file << j.dump(4);

        // Проверка успешности записи
        if (file.fail()) {
            return handleSaveError("Failed to write data to file: " + filename);
        }

        file.close();

        std::cout << "Successfully saved game to: " << filename << std::endl;
        std::cout << "Saved " << gameState.enemies.size() << " enemies and "
            << gameState.towers.size() << " towers" << std::endl;
        return true;

    }
    catch (const std::filesystem::filesystem_error& e) {
        return handleSaveError("Filesystem error: " + std::string(e.what()));
    }
    catch (const nlohmann::json::exception& e) {
        return handleSaveError("JSON serialization error: " + std::string(e.what()));
    }
    catch (const std::exception& e) {
        return handleSaveError("Unexpected error during save: " + std::string(e.what()));
    }
    catch (...) {
        return handleSaveError("Unknown error during save operation");
    }
}

bool JSONSaveManager::loadGameState(GameState& gameState, const std::string& filename) {
    clearLastError();

    try {
        // Проверка существования файла
        if (!saveExists(filename)) {
            return handleLoadError("Save file does not exist: " + filename);
        }

        // Проверка размера файла (защита от огромных файлов)
        auto fileSize = std::filesystem::file_size(filename);
        if (fileSize > 10 * 1024 * 1024) { // 10MB максимум
            return handleLoadError("Save file is too large: " + std::to_string(fileSize) + " bytes");
        }

        std::ifstream file(filename);
        if (!file.is_open()) {
            return handleLoadError("Cannot open file for reading: " + filename);
        }

        // Чтение и парсинг JSON
        nlohmann::json j;
        file >> j;

        // Проверка успешности чтения
        if (file.fail()) {
            return handleLoadError("Failed to read data from file: " + filename);
        }

        file.close();

        // Проверка структуры JSON
        if (!validateJSONStructure(j)) {
            return handleLoadError("Invalid JSON structure in save file");
        }

        // Проверка контрольной суммы на читера
        std::string savedChecksum = j["checksum"];
        nlohmann::json jWithoutChecksum = j;
        jWithoutChecksum.erase("checksum");
        std::string jsonString = jWithoutChecksum.dump();
        std::string calculatedChecksum = calculateChecksum(jsonString);

        if (savedChecksum != calculatedChecksum) {
            return handleLoadError("Checksum validation failed - possible cheating detected");
        }

        // Конвертация JSON в GameState
        if (!JSONToGameState(jWithoutChecksum, gameState)) {
            return handleLoadError("Failed to convert JSON to game state");
        }

        // Валидация загруженных данных
        if (!SaveDataValidator::validateGameState(gameState)) {
            return handleLoadError("Loaded game state failed validation");
        }

        std::cout << "Successfully loaded game from: " << filename << std::endl;
        std::cout << "GameState loaded with player at (" << gameState.playerX
            << "," << gameState.playerY << "), "
            << gameState.enemies.size() << " enemies and "
            << gameState.towers.size() << " towers" << std::endl;
        return true;

    }
    catch (const std::filesystem::filesystem_error& e) {
        return handleLoadError("Filesystem error: " + std::string(e.what()));
    }
    catch (const nlohmann::json::exception& e) {
        return handleLoadError("JSON parsing error: " + std::string(e.what()));
    }
    catch (const std::exception& e) {
        return handleLoadError("Unexpected error during load: " + std::string(e.what()));
    }
    catch (...) {
        return handleLoadError("Unknown error during load operation");
    }
}

bool JSONSaveManager::saveExists(const std::string& filename) {
    try {
        return std::filesystem::exists(filename);
    }
    catch (const std::exception& e) {
        std::cerr << "Error checking file: " << e.what() << std::endl;
        return false;
    }
}

nlohmann::json JSONSaveManager::gameStateToJSON(const GameState& gameState) {
    nlohmann::json j;

    j["turnCount"] = gameState.turnCount;
    j["playerHealth"] = gameState.playerHealth;
    j["playerGold"] = gameState.playerGold;
    j["level"] = gameState.level;
    j["gameRunning"] = gameState.gameRunning;
    j["playerX"] = gameState.playerX;
    j["playerY"] = gameState.playerY;

    // Сохраняем врагов
    nlohmann::json enemiesArray = nlohmann::json::array();
    for (const auto& enemy : gameState.enemies) {
        nlohmann::json enemyJson;
        enemyJson["health"] = enemy.getHealth();
        enemyJson["maxHealth"] = enemy.getMaxHealth();
        enemyJson["damage"] = enemy.getDamage();
        enemyJson["positionX"] = enemy.getPositionX();
        enemyJson["positionY"] = enemy.getPositionY();
        enemyJson["isAlive"] = enemy.isAlive();
        enemiesArray.push_back(enemyJson);
    }
    j["enemies"] = enemiesArray;

    // Сохраняем башни
    nlohmann::json towersArray = nlohmann::json::array();
    for (const auto& tower : gameState.towers) {
        nlohmann::json towerJson;
        towerJson["health"] = tower.getHealth();
        towerJson["maxHealth"] = tower.getMaxHealth();
        towerJson["damage"] = tower.getDamage();
        towerJson["positionX"] = tower.getPositionX();
        towerJson["positionY"] = tower.getPositionY();
        towerJson["isAlive"] = tower.isAlive();
        towersArray.push_back(towerJson);
    }
    j["towers"] = towersArray;

    // Сохраняем размер поля
    j["fieldWidth"] = gameState.field.getWidth();
    j["fieldHeight"] = gameState.field.getHeight();

    nlohmann::json gridArray = nlohmann::json::array();
    for (int y = 0; y < gameState.field.getHeight(); ++y) {
        nlohmann::json rowArray = nlohmann::json::array();
        for (int x = 0; x < gameState.field.getWidth(); ++x) {
            rowArray.push_back(std::string(1, gameState.field.getCell(x, y)));
        }
        gridArray.push_back(rowArray);
    }
    j["grid"] = gridArray;

    // Добавляем контрольную сумму для защиты от читеров
    std::string jsonString = j.dump();
    j["checksum"] = calculateChecksum(jsonString);

    return j;
}

bool JSONSaveManager::JSONToGameState(const nlohmann::json& j, GameState& gameState) {
    try {
        gameState.turnCount = j.value("turnCount", 0);
        gameState.playerHealth = j.value("playerHealth", 10000);
        gameState.playerGold = j.value("playerGold", 100);
        gameState.level = j.value("level", 1);
        gameState.gameRunning = j.value("gameRunning", false);
        gameState.playerX = j.value("playerX", 0);
        gameState.playerY = j.value("playerY", 0);

        // Очищаем существующие векторы
        gameState.enemies.clear();
        gameState.towers.clear();

        // Загружаем врагов
        if (j.contains("enemies") && j["enemies"].is_array()) {
            for (const auto& enemyJson : j["enemies"]) {
                int health = enemyJson.value("health", 50);
                int maxHealth = enemyJson.value("maxHealth", 50);
                int damage = enemyJson.value("damage", 5);
                int posX = enemyJson.value("positionX", 0);
                int posY = enemyJson.value("positionY", 0);
                bool isAlive = enemyJson.value("isAlive", true);

                Enemy enemy(health, damage, posX, posY);

                if (!isAlive || health <= 0) {
                    enemy.takeDamage(maxHealth);
                }
                else if (health < maxHealth) {
                    enemy.takeDamage(maxHealth - health);
                }

                gameState.enemies.push_back(enemy);
            }
        }

        // Загружаем башни
        if (j.contains("towers") && j["towers"].is_array()) {
            for (const auto& towerJson : j["towers"]) {
                int health = towerJson.value("health", 100);
                int maxHealth = towerJson.value("maxHealth", 100);
                int damage = towerJson.value("damage", 10);
                int attackRange = 5; // значение по умолчанию
                int posX = towerJson.value("positionX", 0);
                int posY = towerJson.value("positionY", 0);
                bool isAlive = towerJson.value("isAlive", true);

                EnemyTower tower(health, damage, attackRange, posX, posY);

                if (!isAlive || health <= 0) {
                    tower.takeDamage(maxHealth);
                }
                else if (health < maxHealth) {
                    tower.takeDamage(maxHealth - health);
                }

                gameState.towers.push_back(tower);
            }
        }

        // Загружаем размер поля
        int fieldWidth = j.value("fieldWidth", 15);
        int fieldHeight = j.value("fieldHeight", 15);
        gameState.field.initialize(fieldWidth, fieldHeight);

        if (j.contains("grid") && j["grid"].is_array()) {
            for (int y = 0; y < j["grid"].size(); ++y) {
                const auto& row = j["grid"][y];
                for (int x = 0; x < row.size(); ++x) {
                    std::string cellStr = row[x].get<std::string>();
                    if (!cellStr.empty()) {
                        gameState.field.setCell(x, y, cellStr[0]);
                    }
                }
            }
        }

        std::cout << "GameState loaded with player at (" << gameState.playerX
            << "," << gameState.playerY << "), "
            << gameState.enemies.size() << " enemies and "
            << gameState.towers.size() << " towers" << std::endl;
        std::cout << "Field obstacles restored from save file" << std::endl;

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return false;
    }
}