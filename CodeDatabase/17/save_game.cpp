#include "save_game.h"
#include "direct_damage_spell.h"
#include "trap_spell.h"
#include "area_damage_spell.h"
#include "summon_spell.h"
#include "i_levelup_spell.h"
#include <iostream>

bool SaveGame::save(const GameMap& map, EntityManager* entities) {
    try {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка открытия файла сохранения");
        }
        
        std::time_t saveTime = std::time(nullptr);
        file.write(reinterpret_cast<const char*>(&saveTime), sizeof(saveTime));

        if (!savePlayer(file, entities->getPlayer())) return false;
        if (!saveMap(file, map)) return false;
        if (!saveEntities(file, entities)) return false;

        file.close();
        std::cout << "Игра сохранена\n";
        return true;
        
    } catch (const std::exception& e) {
        std::cout << "Ошибка сохранения: " << e.what() << std::endl;
        return false;
    }
}

bool SaveGame::load(GameMap& map, EntityManager* entities) {
    try {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Файла сохранения нет");
        }

        if (!checkFileIntegrity()) {
            throw std::runtime_error("Файл сохранения поврежден");
        }

        

        std::time_t savedTime;
        file.read(reinterpret_cast<char*>(&savedTime), sizeof(savedTime));

        if (!loadPlayer(file, entities)) return false;
        if (!loadMap(file, map)) return false;
        if (!loadEntities(file, map, entities)) return false;

        file.close();
        std::cout << "Игра загружена\n";
        return true;
        
    } catch (const std::exception& e) {
        std::cout << "Ошибка загрузки: " << e.what() << std::endl;
        return false;
    }
}

bool SaveGame::checkFileIntegrity() {
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) != 0) {
        return false;
    }
    
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    std::time_t savedTime;
    file.read(reinterpret_cast<char*>(&savedTime), sizeof(savedTime));
    
    std::time_t fileModTime = fileInfo.st_mtime;
    file.close();
    
    return savedTime == fileModTime;
}

bool SaveGame::savePlayer(std::ofstream& file, Player& player) {
    try {
        Position playerPos = player.getPosition();
        int playerX = playerPos.getX();
        int playerY = playerPos.getY();
        int playerHealth = player.getHealth();
        int playerLevel = player.getLevel();
        bool skipNextTurn = player.shouldSkipTurn();
        int enemiesKilled = player.getEnemiesKilled();
        bool mode = player.isRangedMode();
        int playerMaxHealth = player.getMaxHealth();
        int damage = player.getdamage();
        
        file.write(reinterpret_cast<const char*>(&playerX), sizeof(playerX));
        file.write(reinterpret_cast<const char*>(&playerY), sizeof(playerY));
        file.write(reinterpret_cast<const char*>(&playerHealth), sizeof(playerHealth));
        file.write(reinterpret_cast<const char*>(&playerLevel), sizeof(playerLevel));
        file.write(reinterpret_cast<const char*>(&skipNextTurn), sizeof(skipNextTurn));
        file.write(reinterpret_cast<const char*>(&enemiesKilled), sizeof(enemiesKilled));
        file.write(reinterpret_cast<const char*>(&mode), sizeof(mode));
        file.write(reinterpret_cast<const char*>(&playerMaxHealth), sizeof(playerMaxHealth));
        file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));

        int spellCount = player.getHand().getSpellCount();
        file.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));
        
        for (int i = 0; i < spellCount; i++) {
            ISpell* spell = player.getHand().getSpell(i);
            std::string spellType = spell->getName();
            int typeLength = spellType.size();
            file.write(reinterpret_cast<const char*>(&typeLength), sizeof(typeLength));
            file.write(spellType.c_str(), typeLength);
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cout << "Ошибка сохранения игрока: " << e.what() << std::endl;
        return false;
    }
}

bool SaveGame::loadPlayer(std::ifstream& file, EntityManager* entities) {
    try {
        int playerX, playerY, playerHealth, playerLevel, enemiesKilled, playerMaxHealth, damage;
        bool skipNextTurn;
        bool mode;
        file.read(reinterpret_cast<char*>(&playerX), sizeof(playerX));
        file.read(reinterpret_cast<char*>(&playerY), sizeof(playerY));
        file.read(reinterpret_cast<char*>(&playerHealth), sizeof(playerHealth));
        file.read(reinterpret_cast<char*>(&playerLevel), sizeof(playerLevel));
        file.read(reinterpret_cast<char*>(&skipNextTurn), sizeof(skipNextTurn));
        file.read(reinterpret_cast<char*>(&enemiesKilled), sizeof(enemiesKilled));
        file.read(reinterpret_cast<char*>(&mode), sizeof(mode));
        file.read(reinterpret_cast<char*>(&playerMaxHealth), sizeof(playerMaxHealth));
        file.read(reinterpret_cast<char*>(&damage), sizeof(damage));


        entities->createPlayer(playerX, playerY, playerHealth);
        Player& loadedPlayer = entities->getPlayer();
        loadedPlayer.setLevel(playerLevel);
        loadedPlayer.setSkipTurn(skipNextTurn);
        loadedPlayer.setEnemiesKilled(enemiesKilled);
        loadedPlayer.setAttackRange(mode);
        loadedPlayer.setMaxHealth(playerMaxHealth);
        loadedPlayer.setDamage(damage);
        if (mode && !loadedPlayer.isRangedMode()) {
            loadedPlayer.switchCombatMode(); 
        }
        

        int spellCount;
        file.read(reinterpret_cast<char*>(&spellCount), sizeof(spellCount));
        
        for (int i = 0; i < spellCount; i++) {
            int typeLength;
            file.read(reinterpret_cast<char*>(&typeLength), sizeof(typeLength));
            
            std::string spellType(typeLength, ' ');
            file.read(&spellType[0], typeLength);
            
            if (spellType == "Прямой урон") {
                loadedPlayer.getHand().addSpell(new DirectDamageSpell());
            } else if (spellType == "Область") {
                loadedPlayer.getHand().addSpell(new AreaDamageSpell());
            } else if (spellType == "Призыв союзника") {
                loadedPlayer.getHand().addSpell(new SummonSpell());
            } else if (spellType == "Ловушка") {
                loadedPlayer.getHand().addSpell(new TrapSpell());
            } else if (spellType == "Улучшение уровня") {
                loadedPlayer.getHand().addSpell(new ILevelUpSpell());
            }
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cout << "Ошибка загрузки игрока: " << e.what() << std::endl;
        return false;
    }
}

bool SaveGame::saveMap(std::ofstream& file, const GameMap& map) {
    try {
        int width = map.getWidth();
        int height = map.getHeight();
        file.write(reinterpret_cast<const char*>(&width), sizeof(width));
        file.write(reinterpret_cast<const char*>(&height), sizeof(height));

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                MapCell cell = map.getCell(Position(x, y));
                MapCell::Type type = cell.getType();
                bool used = cell.isUsed();
                int trapDamage = cell.getTrapDamage();
                
                file.write(reinterpret_cast<const char*>(&type), sizeof(type));
                file.write(reinterpret_cast<const char*>(&used), sizeof(used));
                file.write(reinterpret_cast<const char*>(&trapDamage), sizeof(trapDamage));
            }
        }
        return true;
    } catch (const std::exception& e) {
        std::cout << "Ошибка сохранения карты: " << e.what() << std::endl;
        return false;
    }
}

bool SaveGame::loadMap(std::ifstream& file, GameMap& map) {
    try {
        int width, height;
        file.read(reinterpret_cast<char*>(&width), sizeof(width));
        file.read(reinterpret_cast<char*>(&height), sizeof(height));
        
        map = GameMap(width, height);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                MapCell::Type type;
                bool used;
                int trapDamage;
                
                file.read(reinterpret_cast<char*>(&type), sizeof(type));
                file.read(reinterpret_cast<char*>(&used), sizeof(used));
                file.read(reinterpret_cast<char*>(&trapDamage), sizeof(trapDamage));
                
                MapCell cell(type, used);
                cell.setTrapDamage(trapDamage);
                map.getCell(Position(x, y)) = cell;
            }
        }
        return true;
    } catch (const std::exception& e) {
        std::cout << "Ошибка загрузки карты: " << e.what() << std::endl;
        return false;
    }
}

bool SaveGame::saveEntities(std::ofstream& file, EntityManager* entities) {
    try {

        int enemyCount = entities->getEnemies().size();
        file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
        for (Enemy* enemy : entities->getEnemies()) {
            Position pos = enemy->getPosition();
            int x = pos.getX();
            int y = pos.getY();
            int health = enemy->getHealth();
            bool enemySkip = enemy->shouldSkipTurn();
            int damage = enemy->getDamage();
            file.write(reinterpret_cast<const char*>(&x), sizeof(x));
            file.write(reinterpret_cast<const char*>(&y), sizeof(y));
            file.write(reinterpret_cast<const char*>(&health), sizeof(health));
            file.write(reinterpret_cast<const char*>(&enemySkip), sizeof(enemySkip));
            file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
        }

        int towerCount = entities->getTowers().size();
        file.write(reinterpret_cast<const char*>(&towerCount), sizeof(towerCount));
        for (EnemyTower* tower : entities->getTowers()) {
            Position pos = tower->getPosition();
            int x = pos.getX();
            int y = pos.getY();
            int health = tower->getHealth();
            
            file.write(reinterpret_cast<const char*>(&x), sizeof(x));
            file.write(reinterpret_cast<const char*>(&y), sizeof(y));
            file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        }

        int allyCount = entities->getFollowers().size();
        file.write(reinterpret_cast<const char*>(&allyCount), sizeof(allyCount));
        for (Follower* ally : entities->getFollowers()) {
            Position pos = ally->getPosition();
            int x = pos.getX();
            int y = pos.getY();
            int health = ally->getHealth();
            int damage = ally->getDamage();
            bool allySkip = ally->shouldSkipTurn();
            
            file.write(reinterpret_cast<const char*>(&x), sizeof(x));
            file.write(reinterpret_cast<const char*>(&y), sizeof(y));
            file.write(reinterpret_cast<const char*>(&health), sizeof(health));
            file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
            file.write(reinterpret_cast<const char*>(&allySkip), sizeof(allySkip));
        }

        int buildingCount = entities->getBuildings().size();
        file.write(reinterpret_cast<const char*>(&buildingCount), sizeof(buildingCount));
        for (EnemyBuilding* building : entities->getBuildings()) {
            Position pos = building->getPosition();
            int x = pos.getX();
            int y = pos.getY();
            int spawnInterval = building->getSpawnInterval();
            
            file.write(reinterpret_cast<const char*>(&x), sizeof(x));
            file.write(reinterpret_cast<const char*>(&y), sizeof(y));
            file.write(reinterpret_cast<const char*>(&spawnInterval), sizeof(spawnInterval));
        }

        return true;
    } catch (const std::exception& e) {
        std::cout << "Ошибка сохранения сущностей: " << e.what() << std::endl;
        return false;
    }
}

bool SaveGame::loadEntities(std::ifstream& file, GameMap& map, EntityManager* entities) {
    try {
        Player& loadedPlayer = entities->getPlayer();
        Position playerPos = loadedPlayer.getPosition();
        if (map.isPositionValid(playerPos)) {
            map.getCell(playerPos).setEntity(&loadedPlayer);
            map.getCell(playerPos).setUsed(true);
        }

        int enemyCount;
        file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
        for (int i = 0; i < enemyCount; i++) {
            int x, y, health, damage;
            bool enemySkip;
            file.read(reinterpret_cast<char*>(&x), sizeof(x));
            file.read(reinterpret_cast<char*>(&y), sizeof(y));
            file.read(reinterpret_cast<char*>(&health), sizeof(health));
            file.read(reinterpret_cast<char*>(&enemySkip), sizeof(enemySkip));
            file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
            Enemy* newEnemy = new Enemy(x, y, 1, health, damage);
            newEnemy->setSkipTurn(enemySkip);
            entities->getEnemies().push_back(newEnemy);
            
            Position enemyPos(x, y);
            if (map.isPositionValid(enemyPos)) {
                map.getCell(enemyPos).setEntity(newEnemy);
                map.getCell(enemyPos).setUsed(true);
            }
        }


        int towerCount;
        file.read(reinterpret_cast<char*>(&towerCount), sizeof(towerCount));
        for (int i = 0; i < towerCount; i++) {
            int x, y, health;
            file.read(reinterpret_cast<char*>(&x), sizeof(x));
            file.read(reinterpret_cast<char*>(&y), sizeof(y));
            file.read(reinterpret_cast<char*>(&health), sizeof(health));
            
            EnemyTower* newTower = new EnemyTower(x, y, map.getWidth() - 9);
            entities->getTowers().push_back(newTower);
            
            Position towerPos(x, y);
            if (map.isPositionValid(towerPos)) {
                map.getCell(towerPos).setEntity(newTower);
                map.getCell(towerPos).setUsed(true);
            }
        }

        int allyCount;
        file.read(reinterpret_cast<char*>(&allyCount), sizeof(allyCount));
        for (int i = 0; i < allyCount; i++) {
            int x, y, health, damage;
            bool allySkip;
            file.read(reinterpret_cast<char*>(&x), sizeof(x));
            file.read(reinterpret_cast<char*>(&y), sizeof(y));
            file.read(reinterpret_cast<char*>(&health), sizeof(health));
            file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
            file.read(reinterpret_cast<char*>(&allySkip), sizeof(allySkip));
            
            Follower* newAlly = new Follower(x, y, health, damage);
            newAlly->setSkipTurn(allySkip);
            entities->getFollowers().push_back(newAlly);
            
            Position allyPos(x, y);
            if (map.isPositionValid(allyPos)) {
                map.getCell(allyPos).setEntity(newAlly);
                map.getCell(allyPos).setUsed(true);
            }
        }


        int buildingCount;
        file.read(reinterpret_cast<char*>(&buildingCount), sizeof(buildingCount));
        for (int i = 0; i < buildingCount; i++) {
            int x, y, spawnInterval;
            file.read(reinterpret_cast<char*>(&x), sizeof(x));
            file.read(reinterpret_cast<char*>(&y), sizeof(y));
            file.read(reinterpret_cast<char*>(&spawnInterval), sizeof(spawnInterval));
            
            EnemyBuilding* newBuilding = new EnemyBuilding(x, y, map.getWidth() - 9);
            entities->getBuildings().push_back(newBuilding);
            
            Position buildingPos(x, y);
            if (map.isPositionValid(buildingPos)) {
                map.getCell(buildingPos).setUsed(true);
            }
        }

        return true;
    } catch (const std::exception& e) {
        std::cout << "Ошибка загрузки сущностей: " << e.what() << std::endl;
        return false;
    }
}