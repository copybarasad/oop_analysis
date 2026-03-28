#include "GameSaveManager.h"
#include <sstream>
#include <iostream>
#include <algorithm>


GameSaveManager::GameSaveManager(const std::string& directory) 
    : saveDirectory(directory) {
    ensureSaveDirectoryExists();
}

void GameSaveManager::ensureSaveDirectoryExists() {
    try {
        if (!SimpleFileSystem::exists(saveDirectory)) {
            if (!SimpleFileSystem::createDirectory(saveDirectory)) {
                throw std::runtime_error("Не удалось создать директорию для сохранений: " + saveDirectory);
            }
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Ошибка создания директории сохранений '" + saveDirectory + "': " + e.what());
    }
}

std::string GameSaveManager::getSaveFilePath(const std::string& saveName) const {
    return saveDirectory + "\\" + saveName + ".bin";
}

void GameSaveManager::saveGame(const std::string& saveName, const Player& player, const GameField& field, int currentTurn, int currentLevel) {
    std::string filePath = getSaveFilePath(saveName);
    
    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + filePath);
    }

    try {
        
        uint64_t fileSizePlaceholder = 0;
        file.write(reinterpret_cast<const char*>(&fileSizePlaceholder), sizeof(fileSizePlaceholder));
        
        file.write(reinterpret_cast<const char*>(&currentTurn), sizeof(currentTurn));
        file.write(reinterpret_cast<const char*>(&currentLevel), sizeof(currentLevel));
        
        serializePlayer(file, player);
        serializeGameField(file, field);
        
        file.seekp(0, std::ios::end);
        uint64_t actualFileSize = file.tellp();
        file.seekp(0, std::ios::beg);
        
        file.write(reinterpret_cast<const char*>(&actualFileSize), sizeof(actualFileSize));
        
        if (!file.good()) {
            file.close();
            throw std::runtime_error("Ошибка записи данных в файл: " + filePath);
        }
        file.close();
        std::cout << "Игра успешно сохранена: " << filePath << " (размер: " << actualFileSize << " байт)" << std::endl;
        
    } catch (const std::ios_base::failure& e) {
        file.close();
        throw std::runtime_error("Сбой операции записи в файл '" + filePath + "': " + e.what());
    }
}

void GameSaveManager::loadGame(const std::string& saveName, Player& player, GameField& field, int& loadedTurn, int& loadedLevel) {
    std::string filePath = getSaveFilePath(saveName);
    
    if (!SimpleFileSystem::exists(filePath)) {
        throw std::runtime_error("Файл сохранения не существует: " + filePath);
    }
    
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для чтения: " + filePath);
    }
    
    try {
        file.seekg(0, std::ios::end);
        uint64_t actualFileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        
        
        uint64_t declaredFileSize;
        file.read(reinterpret_cast<char*>(&declaredFileSize), sizeof(declaredFileSize));
        
        
        if (actualFileSize != declaredFileSize) {
            file.close();
            throw std::runtime_error(
                "Файл сохранения поврежден! Размер не совпадает.\n" +
                std::to_string(actualFileSize) + " байт (реальный) != " + 
                std::to_string(declaredFileSize) + " байт (ожидаемый)\n" +
                "Файл был изменен или поврежден."
            );
        }
        

        file.read(reinterpret_cast<char*>(&loadedTurn), sizeof(loadedTurn));
        file.read(reinterpret_cast<char*>(&loadedLevel), sizeof(loadedLevel));

        deserializePlayer(file, player);
        deserializeGameField(file, field);
        
        file.close();
        std::cout << "Игра успешно загружена: " << filePath << std::endl;
        
    } catch (const std::exception& e) {
        file.close();
        throw std::runtime_error("Ошибка загрузки сохранения: " + std::string(e.what()));
    }
}


void GameSaveManager::serializePlayer(std::ostream& os, const Player& player) {
    serializeCharacter(os, player);
    
    int score = player.getScore();
    int coins = player.getCoins();
    int level = player.getLevel();
    int xp = player.getXp();
    AttackType attackType = player.getAttackType();
    int meleeDamage = player.getMeleeDamage(); 
    int rangedDamage = player.getRangedDamage();
    int attackRange = player.getAttackRange();
    
    os.write(reinterpret_cast<const char*>(&score), sizeof(score));
    os.write(reinterpret_cast<const char*>(&coins), sizeof(coins));
    os.write(reinterpret_cast<const char*>(&level), sizeof(level));
    os.write(reinterpret_cast<const char*>(&xp), sizeof(xp));
    os.write(reinterpret_cast<const char*>(&attackType), sizeof(attackType));
    os.write(reinterpret_cast<const char*>(&meleeDamage), sizeof(meleeDamage));
    os.write(reinterpret_cast<const char*>(&rangedDamage), sizeof(rangedDamage));
    os.write(reinterpret_cast<const char*>(&attackRange), sizeof(attackRange));
    
    serializeHandSpell(os, *player.getHandSpell());

}

void GameSaveManager::deserializePlayer(std::istream& is, Player& player) {
    
    deserializeCharacter(is, player);
    int score, coins, level, xp, attackRange;
    AttackType attackType;
    int meleeDamage, rangedDamage;
    
    is.read(reinterpret_cast<char*>(&score), sizeof(score));
    is.read(reinterpret_cast<char*>(&coins), sizeof(coins));
    is.read(reinterpret_cast<char*>(&level), sizeof(level));
    is.read(reinterpret_cast<char*>(&xp), sizeof(xp));
    is.read(reinterpret_cast<char*>(&attackType), sizeof(attackType));
    is.read(reinterpret_cast<char*>(&meleeDamage), sizeof(meleeDamage));
    is.read(reinterpret_cast<char*>(&rangedDamage), sizeof(rangedDamage));
    is.read(reinterpret_cast<char*>(&attackRange), sizeof(attackRange));
    
    player.setScore(score);
    player.setCoins(coins);
    player.setLevel(level);
    player.setXp(xp);
    player.setAttackType(attackType);
    player.setMeleeDamage(meleeDamage);
    player.setRangedDamage(rangedDamage);
    player.setAttackRange(attackRange);
    
    deserializeHandSpell(is, *player.getHandSpell());
    
}

void GameSaveManager::serializeCharacter(std::ostream& os, const Character& character) {
    std::string name = character.getName();
    size_t nameLength = name.length();

    os.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    
    if (nameLength > 0) {
        os.write(name.c_str(), nameLength);
    }
    
    int x = character.getX();
    int y = character.getY();
    int health = character.getHealth();
    int maxHealth = character.getMaxHealth();
    int damage = character.getDamage();
    bool alive = character.isAlive();
    int freezeTime = character.getFreeze_time();
    bool inSaloon = character.getInsaloon();
    
    os.write(reinterpret_cast<const char*>(&x), sizeof(x));
    os.write(reinterpret_cast<const char*>(&y), sizeof(y));
    os.write(reinterpret_cast<const char*>(&health), sizeof(health));
    os.write(reinterpret_cast<const char*>(&maxHealth), sizeof(maxHealth));
    os.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    os.write(reinterpret_cast<const char*>(&alive), sizeof(alive));
    os.write(reinterpret_cast<const char*>(&freezeTime), sizeof(freezeTime));
    os.write(reinterpret_cast<const char*>(&inSaloon), sizeof(inSaloon));
}

void GameSaveManager::deserializeCharacter(std::istream& is, Character& character) {
    
    size_t nameLength;
    is.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    
    if (nameLength > 1000 || nameLength == 0) {
        throw std::runtime_error("Некорректная длина имени персонажа: " + std::to_string(nameLength));
    }

    if (is.eof()) {
        throw std::runtime_error("Неожиданный конец файла при чтении имени персонажа");
    }

    std::string name;
    name.resize(nameLength);
    is.read(&name[0], nameLength);
    
    int x, y, health, maxHealth, damage, freezeTime;
    bool alive, inSaloon;
    
    is.read(reinterpret_cast<char*>(&x), sizeof(x));
    is.read(reinterpret_cast<char*>(&y), sizeof(y));
    is.read(reinterpret_cast<char*>(&health), sizeof(health));
    is.read(reinterpret_cast<char*>(&maxHealth), sizeof(maxHealth));
    is.read(reinterpret_cast<char*>(&damage), sizeof(damage));
    is.read(reinterpret_cast<char*>(&alive), sizeof(alive));
    is.read(reinterpret_cast<char*>(&freezeTime), sizeof(freezeTime));
    is.read(reinterpret_cast<char*>(&inSaloon), sizeof(inSaloon));
    
    character.setName(name);
    character.setPosition(x, y);
    character.setHealth(health);
    character.setMaxHealth(maxHealth);
    character.setDamage(damage);
    character.setAlive(alive);
    character.setFreeze_time(freezeTime);
    character.setinSaloon(inSaloon);
}


void GameSaveManager::serializeGameField(std::ostream& os, const GameField& field) {

    int width = field.getWidth();
    int height = field.getHeight();
    os.write(reinterpret_cast<const char*>(&width), sizeof(width));
    os.write(reinterpret_cast<const char*>(&height), sizeof(height));
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Type_cell cellType = field.getCell(x, y).getType();
            os.write(reinterpret_cast<const char*>(&cellType), sizeof(cellType));
        }
    }
    
    int enemyCount = field.arr_enemies.size();
    os.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
    for (const auto& enemy : field.arr_enemies) {
        serializeCharacter(os, *enemy);
    }
    
    int teammateCount = field.arr_teamates.size();
    os.write(reinterpret_cast<const char*>(&teammateCount), sizeof(teammateCount));
    for (const auto& teammate : field.arr_teamates) {
        serializeCharacter(os, *teammate);
    }
    
    int barracksCount = field.arr_barracks.size();
    os.write(reinterpret_cast<const char*>(&barracksCount), sizeof(barracksCount));
    for (const auto& barracks : field.arr_barracks) {
        serializeBuilding(os, *barracks, Type_cell::BARRACK);
        int cooldown = barracks->getCooldown();
        int currentCooldown = barracks->getCurrentCooldown();
        os.write(reinterpret_cast<const char*>(&cooldown), sizeof(cooldown));
        os.write(reinterpret_cast<const char*>(&currentCooldown), sizeof(currentCooldown));
    }
    
    int saloonCount = field.arr_saloon.size();
    os.write(reinterpret_cast<const char*>(&saloonCount), sizeof(saloonCount));
    for (const auto& saloon : field.arr_saloon) {
        int x = saloon->getX();
        int y = saloon->getY();
        int timeFreeze = saloon->getTime_freeze();
        os.write(reinterpret_cast<const char*>(&x), sizeof(x));
        os.write(reinterpret_cast<const char*>(&y), sizeof(y));
        os.write(reinterpret_cast<const char*>(&timeFreeze), sizeof(timeFreeze));
    }
    
    int towerCount = field.arr_cowboy_towers.size();
    os.write(reinterpret_cast<const char*>(&towerCount), sizeof(towerCount));
    for (const auto& tower : field.arr_cowboy_towers) {
        serializeBuilding(os, *tower, Type_cell::COWBOYTOWER);
        int attackCooldown = tower->getAttackCooldown();
        int attackRange = tower->getAttackRange();
        int currentCooldown = tower->getCurrentCooldown();
        os.write(reinterpret_cast<const char*>(&attackCooldown), sizeof(attackCooldown));
        os.write(reinterpret_cast<const char*>(&attackRange), sizeof(attackRange));
        os.write(reinterpret_cast<const char*>(&currentCooldown), sizeof(currentCooldown));
    }
    
    int trapCount = field.arr_traps.size();
    os.write(reinterpret_cast<const char*>(&trapCount), sizeof(trapCount));
    for (const auto& trap : field.arr_traps) {
        int x = trap->getX();
        int y = trap->getY();
        int damage = trap->getDamage();
        bool active = trap->isActive();
        os.write(reinterpret_cast<const char*>(&x), sizeof(x));
        os.write(reinterpret_cast<const char*>(&y), sizeof(y));
        os.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
        os.write(reinterpret_cast<const char*>(&active), sizeof(active));
    }
    
    serializePits(os, field);
}

void GameSaveManager::deserializeGameField(std::istream& is, GameField& field) {

    int width, height;
    is.read(reinterpret_cast<char*>(&width), sizeof(width));
    is.read(reinterpret_cast<char*>(&height), sizeof(height));
    
    field = GameField(width, height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Type_cell cellType;
            is.read(reinterpret_cast<char*>(&cellType), sizeof(cellType));
            field.setCellType(x, y, cellType);
        }
    }
    
    field.arr_enemies.clear();
    field.arr_teamates.clear();
    field.arr_barracks.clear();
    field.arr_saloon.clear();
    field.arr_cowboy_towers.clear();
    field.arr_traps.clear();
    field.arr_pit.clear();
    
    int enemyCount;
    is.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
    for (int i = 0; i < enemyCount; ++i) {
        auto enemy = std::make_unique<Enemy>("", 0, 0, 0, 0);
        deserializeCharacter(is, *enemy);
        field.arr_enemies.push_back(std::move(enemy));
    }
    
    int teammateCount;
    is.read(reinterpret_cast<char*>(&teammateCount), sizeof(teammateCount));
    for (int i = 0; i < teammateCount; ++i) {
        auto teammate = std::make_unique<Teamate>("", 0, 0, 0, 0);
        deserializeCharacter(is, *teammate);
        field.arr_teamates.push_back(std::move(teammate));
    }
    
    int barracksCount;
    is.read(reinterpret_cast<char*>(&barracksCount), sizeof(barracksCount));
    for (int i = 0; i < barracksCount; ++i) {
        auto barracks = std::make_unique<Barracks>(0, 0, 0);
        deserializeBuilding(is, *barracks);
        int cooldown, currentCooldown;
        is.read(reinterpret_cast<char*>(&cooldown), sizeof(cooldown));
        is.read(reinterpret_cast<char*>(&currentCooldown), sizeof(currentCooldown));
        barracks->setCooldown(cooldown);
        barracks->setCurrentCooldown(currentCooldown);
        field.arr_barracks.push_back(std::move(barracks));
    }
    
    int saloonCount;
    is.read(reinterpret_cast<char*>(&saloonCount), sizeof(saloonCount));
    for (int i = 0; i < saloonCount; ++i) {
        int x, y, timeFreeze;
        is.read(reinterpret_cast<char*>(&x), sizeof(x));
        is.read(reinterpret_cast<char*>(&y), sizeof(y));
        is.read(reinterpret_cast<char*>(&timeFreeze), sizeof(timeFreeze));
        
        auto saloon = std::make_unique<Saloon>(x, y);
        saloon->setTime_freeze(timeFreeze);
        field.arr_saloon.push_back(std::move(saloon));
    }
    
    int towerCount;
    is.read(reinterpret_cast<char*>(&towerCount), sizeof(towerCount));
    for (int i = 0; i < towerCount; ++i) {
        auto tower = std::make_unique<CowboyTower>(0, 0, 0, 0, 0);
        deserializeBuilding(is, *tower);
        int attackCooldown, attackRange, currentCooldown;
        is.read(reinterpret_cast<char*>(&attackCooldown), sizeof(attackCooldown));
        is.read(reinterpret_cast<char*>(&attackRange), sizeof(attackRange));
        is.read(reinterpret_cast<char*>(&currentCooldown), sizeof(currentCooldown));
        
        tower->setAttackCooldown(attackCooldown);
        tower->setAttackRange(attackRange);
        tower->setCurrentCooldown(currentCooldown);
        field.arr_cowboy_towers.push_back(std::move(tower));
    }
    
    int trapCount;
    is.read(reinterpret_cast<char*>(&trapCount), sizeof(trapCount));
    for (int i = 0; i < trapCount; ++i) {
        int x, y, damage;
        bool active;
        is.read(reinterpret_cast<char*>(&x), sizeof(x));
        is.read(reinterpret_cast<char*>(&y), sizeof(y));
        is.read(reinterpret_cast<char*>(&damage), sizeof(damage));
        is.read(reinterpret_cast<char*>(&active), sizeof(active));
        
        auto trap = std::make_unique<Trap>(x, y, damage);
        field.arr_traps.push_back(std::move(trap));
    }
    
    deserializePits(is, field);
}

void GameSaveManager::serializePits(std::ostream& os, const GameField& field) {
    int pitCount = field.arr_pit.size();
    os.write(reinterpret_cast<const char*>(&pitCount), sizeof(pitCount));
    
    for (const auto& pit : field.arr_pit) {
        int x = pit->getX();
        int y = pit->getY();
        os.write(reinterpret_cast<const char*>(&x), sizeof(x));
        os.write(reinterpret_cast<const char*>(&y), sizeof(y));
    }
}

void GameSaveManager::deserializePits(std::istream& is, GameField& field) {
    int pitCount;
    is.read(reinterpret_cast<char*>(&pitCount), sizeof(pitCount));
    
    for (int i = 0; i < pitCount; ++i) {
        int x, y;
        is.read(reinterpret_cast<char*>(&x), sizeof(x));
        is.read(reinterpret_cast<char*>(&y), sizeof(y));
        
        auto pit = std::make_unique<Pit>(x, y);
        field.arr_pit.push_back(std::move(pit));
    }
}

void GameSaveManager::serializeHandSpell(std::ostream& os, const HandSpell& handSpell) {
    
    int cntMegaShotgun = handSpell.getCntMegaShotgun();
    int cntBulletRain = handSpell.getCntBulletRain();
    int cntCheapWhiskey = handSpell.getCntCheapWhiskey();
    int cntJohnnysGhost = handSpell.getCntJohnnysGhost();
    int cntCurrentTeamates = handSpell.getGhostJhonny()->getCurrentSpawns();
    int cntUpdateSpell = handSpell.getCntUpdateSpell();
    int cntTotalSpells = handSpell.getCntTotalSpells();
    int currentSpellLevel = handSpell.getUpdateSpell()->getLevel(); 
    
    os.write(reinterpret_cast<const char*>(&cntMegaShotgun), sizeof(cntMegaShotgun));
    os.write(reinterpret_cast<const char*>(&cntBulletRain), sizeof(cntBulletRain));
    os.write(reinterpret_cast<const char*>(&cntCheapWhiskey), sizeof(cntCheapWhiskey));
    os.write(reinterpret_cast<const char*>(&cntJohnnysGhost), sizeof(cntJohnnysGhost));
    os.write(reinterpret_cast<const char*>(&cntCurrentTeamates), sizeof(cntCurrentTeamates));
    os.write(reinterpret_cast<const char*>(&cntUpdateSpell), sizeof(cntUpdateSpell));
    os.write(reinterpret_cast<const char*>(&cntTotalSpells), sizeof(cntTotalSpells));
    os.write(reinterpret_cast<const char*>(&currentSpellLevel), sizeof(currentSpellLevel));
}

void GameSaveManager::deserializeHandSpell(std::istream& is, HandSpell& handSpell) {
    int cntMegaShotgun, cntBulletRain, cntCheapWhiskey, cntJohnnysGhost, cntCurrentTeamates, cntUpdateSpell, cntTotalSpells, currentSpellLevel;
    
    is.read(reinterpret_cast<char*>(&cntMegaShotgun), sizeof(cntMegaShotgun));
    is.read(reinterpret_cast<char*>(&cntBulletRain), sizeof(cntBulletRain));
    is.read(reinterpret_cast<char*>(&cntCheapWhiskey), sizeof(cntCheapWhiskey));
    is.read(reinterpret_cast<char*>(&cntJohnnysGhost), sizeof(cntJohnnysGhost));
    is.read(reinterpret_cast<char*>(&cntCurrentTeamates), sizeof(cntCurrentTeamates));
    is.read(reinterpret_cast<char*>(&cntUpdateSpell), sizeof(cntUpdateSpell));
    is.read(reinterpret_cast<char*>(&cntTotalSpells), sizeof(cntTotalSpells));
    is.read(reinterpret_cast<char*>(&currentSpellLevel), sizeof(currentSpellLevel));
    
    handSpell.setCntMegaShotgun(cntMegaShotgun);
    handSpell.setCntBulletRain(cntBulletRain);
    handSpell.setCntCheapWhiskey(cntCheapWhiskey);
    handSpell.setCntJohnnysGhost(cntJohnnysGhost);    
    handSpell.setCntUpdateSpell(cntUpdateSpell);
    handSpell.setCntTotalSpells(cntTotalSpells); 
    handSpell.setSpellLevel(currentSpellLevel);
    handSpell.getGhostJhonny()->setCurrentSpawns(cntCurrentTeamates);
}


void GameSaveManager::serializeBuilding(std::ostream& os, const Building& building, Type_cell buildingType) {
    int x = building.getX();
    int y = building.getY();
    int health = building.getHealth();
    bool notDestroyed = building.getIsNotDestroyed();
    
    os.write(reinterpret_cast<const char*>(&x), sizeof(x));
    os.write(reinterpret_cast<const char*>(&y), sizeof(y));
    os.write(reinterpret_cast<const char*>(&health), sizeof(health));
    os.write(reinterpret_cast<const char*>(&notDestroyed), sizeof(notDestroyed));
    os.write(reinterpret_cast<const char*>(&buildingType), sizeof(buildingType));
}

void GameSaveManager::deserializeBuilding(std::istream& is, Building& building) {
    int x, y, health;
    bool notDestroyed;
    Type_cell buildingType;
    
    is.read(reinterpret_cast<char*>(&x), sizeof(x));
    is.read(reinterpret_cast<char*>(&y), sizeof(y));
    is.read(reinterpret_cast<char*>(&health), sizeof(health));
    is.read(reinterpret_cast<char*>(&notDestroyed), sizeof(notDestroyed));
    is.read(reinterpret_cast<char*>(&buildingType), sizeof(buildingType));
    
    building.setPosition(x, y);
    building.setHealth(health);
    building.setIsNotDestroyed(notDestroyed);
}

std::vector<std::string> GameSaveManager::getAvailableSaves() const {
    std::vector<std::string> saves;
    
    try {
        saves = SimpleFileSystem::getFilesInDirectory(saveDirectory, ".bin");
    } catch (const std::exception& e) {
        std::cerr << "Ошибка чтения сохранной директории: " << e.what() << std::endl;
    }
    
    return saves;
}

bool GameSaveManager::saveExists(const std::string& saveName) const {
    return SimpleFileSystem::exists(getSaveFilePath(saveName));
}

