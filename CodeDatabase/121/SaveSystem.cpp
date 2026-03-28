#include "SaveSystem.h"
#include <iostream>
#include <filesystem>

SaveSystem::SaveSystem(std::shared_ptr<TextSystem> textSystem, const std::string& filename) 
    : textSystem(textSystem), saveFilename(filename) {}

void SaveSystem::saveGame(const LevelController& level, int currentLevelNumber, 
                         GameState currentState, int turnCount) {
    try {
        // Создаем резервную копию существующего сохранения
        backupExistingSave();
        
        // Создаем JSON объект со всей информацией
        json gameData = serializeGameData(level, currentLevelNumber, currentState, turnCount);
        
        // Записываем в файл с красивым форматированием
        std::ofstream file(saveFilename);
        if (!file.is_open()) {
            throw SaveLoadException("Не удалось открыть файл для сохранения: " + saveFilename);
        }
        
        file << gameData.dump(4); // 4 пробела для отступов
        file.close();
        
        std::cout << textSystem->getSaveSuccessMessage() << std::endl;
        
    } catch (const std::exception& e) {
        throw SaveLoadException("Ошибка сохранения: " + std::string(e.what()));
    }
}

void SaveSystem::loadGame(GameConfig& config, LevelController& level, int& currentLevelNumber, 
                         GameState& currentState, int& turnCount) {
    try {
        // Читаем JSON из файла
        std::ifstream file(saveFilename);
        if (!file.is_open()) {
            throw SaveLoadException("Не удалось открыть файл для загрузки: " + saveFilename);
        }
        
        json gameData;
        file >> gameData;
        file.close();
        
        // Проверяем валидность файла сохранения
        validateSaveFile(gameData);
        
        // Заполняем переданные параметры
        deserializeGameData(gameData, config, level, currentLevelNumber, currentState, turnCount);
        
        std::cout << textSystem->getLoadSuccessMessage() << std::endl;
        
    } catch (const std::exception& e) {
        throw SaveLoadException("Ошибка загрузки: " + std::string(e.what()));
    }
}

void SaveSystem::backupExistingSave() const {
    namespace fs = std::filesystem;
    
    if (fs::exists(saveFilename)) {
        std::string backupName = saveFilename + ".backup";
        try {
            fs::copy_file(saveFilename, backupName, fs::copy_options::overwrite_existing);
        } catch (const fs::filesystem_error& e) {
            // Игнорируем ошибки резервного копирования - не критично
        }
    }
}

void SaveSystem::validateSaveFile(const json& j) const {
    // Проверяем обязательные поля
    if (!j.contains("metadata") || !j.contains("game_state")) {
        throw SaveLoadException("Файл сохранения поврежден: отсутствуют обязательные поля");
    }
    
    // Проверяем версию
    int version = j["metadata"]["version"];
    if (version != 1) {
        throw SaveLoadException("Неподдерживаемая версия файла сохранения: " + std::to_string(version));
    }
    
    // Проверяем базовую структуру
    if (!j["game_state"].contains("current_level") || 
        !j["game_state"].contains("state") || 
        !j["game_state"].contains("turn_count")) {
        throw SaveLoadException("Файл сохранения поврежден: неполные данные о состоянии игры");
    }
}

// СЕРИАЛИЗАЦИЯ (

json SaveSystem::serializeGameData(const LevelController& level, int currentLevelNumber, 
                                  GameState currentState, int turnCount) const {
    json j;
    
    // Метаданные
    j["metadata"]["version"] = 1;
    j["metadata"]["game_name"] = "Tactical Battle";
    j["metadata"]["save_type"] = "full_save";
    j["metadata"]["timestamp"] = std::time(nullptr);
    
    // Информация о состоянии игры
    j["game_state"]["current_level"] = currentLevelNumber;
    j["game_state"]["state"] = static_cast<int>(currentState);
    j["game_state"]["turn_count"] = turnCount;
    
    // Сохраняем уровень
    j["level"]["type"] = static_cast<int>(level.getLevelType());
    j["level"]["player"] = serializePlayer(level.getPlayer());
    j["level"]["field"] = serializeGameField(level.getField());
    j["level"]["enemies"] = serializeEnemies(level.getEnemies());
    j["level"]["buildings"] = serializeBuildings(level.getBuildings());
    j["level"]["towers"] = serializeTowers(level.getTowers());
    
    return j;
}

json SaveSystem::serializePlayer(const Player& player) const {
    json j;
    
    j["health"] = player.getHealth();
    j["damage"] = player.getDamage();
    j["score"] = player.getScore();
    j["slowed"] = player.getSlowed();
    j["can_act"] = player.getCanAct();
    j["battle_mode"] = static_cast<int>(player.getBattleMode());
    j["position"] = serializePosition(player.getPosition());
    j["spell_hand"] = serializeSpellHand(player.getHand());
    
    return j;
}

json SaveSystem::serializeGameField(const GameField& field) const {
    json j;
    
    j["width"] = field.getWidth();
    j["height"] = field.getHeight();
    
    // Сохраняем все клетки
    json cells = json::array();
    for (int x = 0; x < field.getWidth(); x++) {
        for (int y = 0; y < field.getHeight(); y++) {
            Position pos(x, y);
            const Cell& cell = field.getCell(pos);
            
            json cellData;
            cellData["x"] = x;
            cellData["y"] = y;
            cellData["type"] = static_cast<int>(cell.getType());
            cellData["occupied"] = cell.isOccupied();
            
            cells.push_back(cellData);
        }
    }
    
    j["cells"] = cells;
    return j;
}

json SaveSystem::serializeEnemies(const std::vector<Enemy>& enemies) const {
    json j = json::array();
    
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) { // Сохраняем только живых врагов
            json enemyData;
            enemyData["health"] = enemy.getHealth();
            enemyData["position"] = serializePosition(enemy.getPosition());
            
            j.push_back(enemyData);
        }
    }
    
    return j;
}

json SaveSystem::serializeBuildings(const std::vector<EnemyBuilding>& buildings) const {
    json j = json::array();
    
    for (const auto& building : buildings) {
        if (building.isAlive()) {
            json buildingData;
            buildingData["health"] = building.getHealth();
            buildingData["position"] = serializePosition(building.getPosition());
            buildingData["current_cooldown"] = building.getCurrentCooldown();
            
            j.push_back(buildingData);
        }
    }
    
    return j;
}

json SaveSystem::serializeTowers(const std::vector<EnemyTower>& towers) const {
    json j = json::array();
    
    for (const auto& tower : towers) {
        if (tower.isAlive()) {
            json towerData;
            towerData["health"] = tower.getHealth();
            towerData["position"] = serializePosition(tower.getPosition());
            towerData["current_spell_cooldown"] = tower.getCurrentSpellCooldown();
            towerData["can_cast_spell"] = tower.canCast();
            
            j.push_back(towerData);
        }
    }
    
    return j;
}

json SaveSystem::serializeSpellHand(const SpellHand& hand) const {
    json j = json::array();
    
    for (size_t i = 0; i < hand.getSpellCount(); i++) {
        if (const Spell* spell = hand.getSpell(i)) {
            std::string spellType;
            if (dynamic_cast<const DirectDamageSpell*>(spell)) {
                spellType = "direct_damage";
            } else if (dynamic_cast<const AreaDamageSpell*>(spell)) {
                spellType = "area_damage";
            } else if (dynamic_cast<const TrapSpell*>(spell)) {
                spellType = "trap";
            } else {
                spellType = "unknown";
            }
            
            j.push_back(spellType);
        }
    }
    
    return j;
}

json SaveSystem::serializePosition(const Position& pos) const {
    json j;
    j["x"] = pos.first;
    j["y"] = pos.second;
    return j;
}


// ДЕСЕРИАЛИЗАЦИЯ 

void SaveSystem::deserializeGameData(const json& j, GameConfig& config, LevelController& level,
                                   int& currentLevelNumber, GameState& currentState, int& turnCount) const {
    // Восстанавливаем базовую информацию
    currentLevelNumber = j["game_state"]["current_level"];
    currentState = static_cast<GameState>(j["game_state"]["state"]);
    turnCount = j["game_state"]["turn_count"];
    
    // Восстанавливаем уровень
    if (j.contains("level")) {
        deserializePlayer(level.getPlayer(), j["level"]["player"]);
        deserializeGameField(level.getField(), j["level"]["field"]);
        deserializeEnemies(level.getEnemies(), j["level"]["enemies"], config);
        deserializeBuildings(level.getBuildings(), j["level"]["buildings"], config);
        deserializeTowers(level.getTowers(), j["level"]["towers"], config);
    }
}

void SaveSystem::deserializePlayer(Player& player, const json& j) const {
    player.setHealth(j["health"]);
    player.setDamage(j["damage"]);
    player.setScore(j["score"]);
    player.setSlowed(j["slowed"]);
    player.setCanAct(j["can_act"]);
    player.setBattleMode(static_cast<BattleMode>(j["battle_mode"]));
    player.setPosition(deserializePosition(j["position"]));
    deserializeSpellHand(player.getHand(), j["spell_hand"]);
}

void SaveSystem::deserializeGameField(GameField& field, const json& j) const {
    int width = j["width"];
    int height = j["height"];
    
    // Проверяем совпадение размеров
    if (width != field.getWidth() || height != field.getHeight()) {
        throw SaveLoadException("Размеры поля в сохранении не совпадают с текущими");
    }
    
    // Восстанавливаем все клетки
    for (const auto& cellData : j["cells"]) {
        int x = cellData["x"];
        int y = cellData["y"];
        
        if (x >= 0 && x < width && y >= 0 && y < height) {
            Position pos(x, y);
            Cell& cell = field.getCell(pos);
            cell.setType(static_cast<CellType>(cellData["type"]));
            cell.setOccupied(cellData["occupied"]);
        }
    }
}

void SaveSystem::deserializeEnemies(std::vector<Enemy>& enemies, const json& j, const GameConfig& config) const {
    enemies.clear();
    
    for (const auto& enemyData : j) {
        Position pos = deserializePosition(enemyData["position"]);
        Enemy enemy(pos, config.getEnemyConfig());
        enemy.setHealth(enemyData["health"]);
        enemies.push_back(enemy);
    }
}

void SaveSystem::deserializeBuildings(std::vector<EnemyBuilding>& buildings, const json& j, const GameConfig& config) const {
    buildings.clear();
    
    for (const auto& buildingData : j) {
        Position pos = deserializePosition(buildingData["position"]);
        EnemyBuilding building(pos, config.getBuildingConfig());
        building.setHealth(buildingData["health"]);
        buildings.push_back(building);
    }
}

void SaveSystem::deserializeTowers(std::vector<EnemyTower>& towers, const json& j, const GameConfig& config) const {
    towers.clear();
    
    for (const auto& towerData : j) {
        Position pos = deserializePosition(towerData["position"]);
        EnemyTower tower(pos, config.getTowerConfig());
        tower.setHealth(towerData["health"]);
        towers.push_back(tower);
    }
}

void SaveSystem::deserializeSpellHand(SpellHand& hand, const json& j) const {
    // Очищаем текущие заклинания
    while (hand.getSpellCount() > 0) {
        hand.removeSpell(0);
    }
    
    for (const auto& spellType : j) {
        std::shared_ptr<Spell> spell;
        
        if (spellType == "direct_damage") {
            spell = std::make_shared<DirectDamageSpell>();
        } else if (spellType == "area_damage") {
            spell = std::make_shared<AreaDamageSpell>();
        } else if (spellType == "trap") {
            spell = std::make_shared<TrapSpell>();
        }
        
        if (spell && hand.hasSpace()) {
            hand.addSpell(spell);
        }
    }
}

Position SaveSystem::deserializePosition(const json& j) const {
    return Position(j["x"], j["y"]);
}
